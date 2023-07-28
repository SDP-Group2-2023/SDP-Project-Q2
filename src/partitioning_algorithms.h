#ifndef PARTITIONING_ALGORITHMS_H
#define PARTITIONING_ALGORITHMS_H

#include "Graph.h"

int sequential_partitioning(Graph graph, int num_partitions);
int parallel_partitioning(const Graph& graph, int num_partitions, int num_threads);

#endif
