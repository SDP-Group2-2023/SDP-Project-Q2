#ifndef GRAPHPARTITIONING_PARTITIONING_H
#define GRAPHPARTITIONING_PARTITIONING_H

#include "Graph.h"

Graph* loadFromFile(const string& path, int num_threads);
Graph* coarseGraph(Graph* graph);
void partitioning(Graph*g, int requestedPartitions);
void kernighanLin(Graph* graph, int num_partitions, vector<int>& partitions);


#endif //GRAPHPARTITIONING_PARTITIONING_H
