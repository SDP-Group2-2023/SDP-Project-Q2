#include "Graph.h"






vector<int> kernighanLin(Graph* graph, int num_partitions){
    vector<int> partitions(graph->V, 0);

    for(int i = 0; i<graph->V; i++){
        partitions[i] = i % num_partitions;
    }


    return partitions;
}