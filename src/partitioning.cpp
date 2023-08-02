#include "Graph.h"
#include "partitioning.h"
#include <vector>
#include <iostream>

using namespace std;
void partitioning(Graph*g, int requestedPartitions){
    int actual_num_partitions = g->V;

    vector<Graph*> allGraphs;
    allGraphs.push_back(g);

    int iterations = 0;
    while( actual_num_partitions > requestedPartitions * 15 && iterations++ < 50){
        cout << "Iteration " << iterations << endl;
        Graph* coarsedGraph = coarseGraph(allGraphs.back(), 0);
        //coarsedGraph->print();
        actual_num_partitions = coarsedGraph->V;
        allGraphs.push_back(coarsedGraph);
    }

    auto partitions = kernighanLin(allGraphs.back(), 50);
    for(int i = 0; i<partitions.size(); i++){
        cout << "Node " << i << " in partition " << partitions[i] << endl;
    }

    for(int i = 1; i<allGraphs.size(); i++){
        allGraphs[i]->free();
    }
}