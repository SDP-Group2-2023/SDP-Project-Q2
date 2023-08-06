#include "Graph.h"
#include "partitioning.h"
#include <vector>
#include <iostream>

using namespace std;
vector<int> uncoarsenGraph(Graph* g, vector<int>& partitions){
    vector<int> newPartitions(g->V);

    for(auto&n : g->nodes)
        newPartitions[n->id] = partitions[n->child->id];

    return newPartitions;
}

void partitioning(Graph*g, int requestedPartitions){
    int actual_num_partitions = g->V;

    vector<Graph*> allGraphs;
    allGraphs.push_back(g);

    int iterations = 0;
    while( actual_num_partitions > requestedPartitions * 15  && iterations++ < 50){
        cout << "Iteration " << iterations << endl;
        Graph* coarsedGraph = coarseGraph(allGraphs.back(), 0);
        //coarsedGraph->print();
        actual_num_partitions = coarsedGraph->V;
        allGraphs.push_back(coarsedGraph);
    }

    Graph* coarsestGraph = allGraphs[allGraphs.size()-1];
    vector<int> partitions(coarsestGraph->V);
    for(int i = 0; i<coarsestGraph->V; i++){
        partitions[i] = i % requestedPartitions;
    }

    cout << "Before requested partitions: " << requestedPartitions << endl;
    cout << "Before address of partitions: " << &partitions << endl;
    kernighanLin(coarsestGraph, requestedPartitions, partitions);

    for(auto i = (int)allGraphs.size()-2; i>=0; i--){
        partitions = uncoarsenGraph(allGraphs[i], partitions);
        kernighanLin(allGraphs[i], requestedPartitions, partitions);
    }

    for(int i = 0; i<partitions.size(); i++){
        cout << "Node " << i << " in partition " << partitions[i] << endl;
    }

    for(int i = 1; i<allGraphs.size(); i++){
        delete allGraphs[i];
    }
}

