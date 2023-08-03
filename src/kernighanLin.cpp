#include "Graph.h"
#include <iostream>

using namespace std;

int countPartitionWeight(Graph* graph, int partition, vector<int>& partitions){
    int weight = 0;
    for(auto&n : graph->nodes){
        if(partitions[n->id] == partition){
            weight += n->weight;
        }
    }
    return weight;
}

long calculateCutSize(Graph* graph, vector<int>& partitions){
    long cutsize = 0;
    for(auto&n : graph->nodes){
        for(auto & edge : n->edges) {
            int source, dest;
            source = edge->node1->id;
            dest = edge->node2->id;
            if(partitions[source] != partitions[dest])
                cutsize += edge->weight;
        }
    }
    return cutsize;
}

void kernighanLin(Graph* graph, int num_partitions, vector<int>& partitions){

    //calcolo dei gain
    vector<int> gain(graph->V);
    for(auto&n: graph->nodes){
        gain[n->id] = 0;
        for(auto & edge : n->edges) {
            gain[n->id] += edge->weight;
        }
    }

    vector<int> partitionsWeight(num_partitions);
    for(int i = 0; i<num_partitions; i++){
        partitionsWeight[i] = countPartitionWeight(graph, i, partitions);
        //cout << "Partition " << i << " weight: " << partitionsWeight[i] << endl;
    }


    //calcolo della cutsize
    long cutsize = calculateCutSize(graph, partitions);
    cout << "Cutsize: " << cutsize << endl;
}
