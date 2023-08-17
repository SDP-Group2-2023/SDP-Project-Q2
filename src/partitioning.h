#ifndef GRAPHPARTITIONING_PARTITIONING_H
#define GRAPHPARTITIONING_PARTITIONING_H

#include "Graph.h"

Graph* loadFromFile(const string& path);
Graph* coarseGraph(Graph* graph);
void parallel_partitioning(Graph* graph, int requestedPartitions, int num_threads);
void sequential_partitioning(Graph*g, int requestedPartitions);
void kernighanLin(Graph* graph, int num_partitions, vector<int>& partitions);


#endif //GRAPHPARTITIONING_PARTITIONING_H
