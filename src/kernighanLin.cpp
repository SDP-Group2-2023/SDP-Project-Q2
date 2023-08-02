#include "Graph.h"






void kernighanLin(Graph* graph, int num_partitions, vector<int>& partitions){

    for(int i = 0; i<graph->V; i++){
        partitions[i] = i % num_partitions;
    }
}