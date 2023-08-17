#include "Graph.h"
#include "partitioning.h"
#include <vector>
#include <iostream>
#include <thread>

void uncoarsen_graph_step(Graph*g, vector<int>&partitions, vector<int>&newPartitions, int start, int end) {
    for (int i = start; i < end; i++)
        newPartitions[g->nodes[i]->id] = partitions[g->nodes[i]->child->id];
}

vector<int> uncoarsen_graph_p(Graph*g, vector<int>&partitions, int num_thread){
    int num_nodes = g->V;
    vector<int> newPartitions(num_nodes);
    vector<thread> threads;
    int step = num_nodes / num_thread;
    int start;
    int end;
    for(int i = 0; i<num_thread - 1; i++){
        start = i * step;
        end = start + step - 1;
        threads.emplace_back(uncoarsen_graph_step, g, ref(partitions), ref(newPartitions), start, end);
    }
    start = (num_thread - 1) * step;
    end = num_nodes - 1;
    threads.emplace_back(uncoarsen_graph_step, g, ref(partitions),ref(newPartitions), start, end);

    for(auto&t : threads)
        t.join();

    return newPartitions;
}

void parallel_partitioning(Graph* g, int requestedPartitions, int num_threads){
    int actual_num_partitions = g->V;

    vector<Graph*> allGraphs;
    allGraphs.push_back(g);

    int iterations = 0;
    while( actual_num_partitions > requestedPartitions * 15  && iterations++ < 50){
        cout << "Iteration " << iterations << endl;
        Graph* coarsedGraph = coarseGraph(allGraphs.back());
        //coarsedGraph->print();
        actual_num_partitions = coarsedGraph->V;
        cout << "Actual number of partitions: " << actual_num_partitions << endl;
        allGraphs.push_back(coarsedGraph);
    }

    Graph* coarsestGraph = allGraphs[allGraphs.size()-1];
    vector<int> partitions(coarsestGraph->V);
    for(int i = 0; i<coarsestGraph->V; i++){
        partitions[i] = i % requestedPartitions;
    }

    kernighanLin(coarsestGraph, requestedPartitions, partitions);

    for (auto i = (int) allGraphs.size() - 2; i >= 0; i--) {
        partitions = uncoarsen_graph_p(allGraphs[i], partitions, num_threads);
        cout << "Uncoarsening step " <<  allGraphs.size() - i - 1 << endl;
        allGraphs[i]->partitions_size = allGraphs[i + 1]->partitions_size;
        kernighanLin(allGraphs[i], requestedPartitions, partitions);
    }

    for(int i = 0; i<partitions.size(); i++){
        cout << "Node " << i << " in partition " << partitions[i] << endl;
    }

    for(int i = 1; i<allGraphs.size(); i++)
        delete allGraphs[i];
}