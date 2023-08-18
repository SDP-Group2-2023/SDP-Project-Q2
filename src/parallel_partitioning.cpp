#include "Graph.h"
#include "partitioning.h"
#include <vector>
#include <iostream>
#include <thread>

void uncoarsen_graph_step(Graph*g, vector<int>&partitions,
                          vector<int>&newPartitions, int num_nodes,
                          int start, int step) {
    int i = start;
    while(i < num_nodes) {
        newPartitions[g->nodes[i]->id] = partitions[g->nodes[i]->child->id];
        i += step;
    }
}

vector<int> uncoarsen_graph_p(Graph*g, vector<int>&partitions, int num_thread){
    int num_nodes = g->V;
    vector<int> newPartitions(num_nodes);
    vector<thread> threads;

    for(int i = 0; i < num_thread; i++) {
        threads.push_back(thread(uncoarsen_graph_step, g, ref(partitions), ref(newPartitions), num_nodes, i, num_thread));
    }
    for(auto &t : threads) {
        t.join();
    }

    return newPartitions;
}

void parallel_partitioning(Graph* g, int requestedPartitions, int num_threads){
    int actual_num_partitions = g->V;

    vector<Graph*> allGraphs;
    allGraphs.push_back(g);

    int iterations = 0;
    while( actual_num_partitions > requestedPartitions * 15  && iterations++ < 50){
        cout << "Iteration " << iterations << endl;
        auto start = chrono::high_resolution_clock::now();
        Graph* coarsedGraph = coarseGraph_s(allGraphs.back());
        auto end = chrono::high_resolution_clock::now();
        cout << "Coarsening time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        //coarsedGraph->print();
        actual_num_partitions = coarsedGraph->V;
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