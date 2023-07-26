#include "Graph.h"

int sequential_partitioning(const Graph& graph, int num_partitions);
int parallel_partitioning(const Graph& graph, int num_partitions, int num_threads);