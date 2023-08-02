#ifndef GRAPHPARTITIONING_PARTITIONING_H
#define GRAPHPARTITIONING_PARTITIONING_H

#include "Graph.h"

Graph* loadFromFile(const string& path);
Graph* coarseGraph(Graph* graph, int num_partitions);


#endif //GRAPHPARTITIONING_PARTITIONING_H
