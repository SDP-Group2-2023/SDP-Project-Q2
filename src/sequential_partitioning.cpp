#include "Graph.h"
#include "partitioning.h"
#include <vector>
#include <iostream>

using namespace std;
/**
 * @brief Uncoarsen a graph based on the partitions of the coarsed graph
 * @param g Graph to uncoarsen
 * @param partitions vector of int with the partitions of the coarsed graph
 * @return vector of the partitions of the uncompressed graph
 */
vector<int> uncoarsenGraph(Graph* g, vector<int>& partitions){
    vector<int> newPartitions(g->V);

    for(auto&n : g->nodes)
        newPartitions[n->id] = partitions[n->child->id];

    return newPartitions;
}

/**
 * @brief Partition a graph using the Multilevel Kernighan-Lin algorithm
 * @param g Graph to coarse
 * @param requestedPartitions number of partitions requested
 */
void sequential_partitioning(Graph*g, int requestedPartitions){
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
        partitions = uncoarsenGraph(allGraphs[i], partitions);
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

