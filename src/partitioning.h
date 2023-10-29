#ifndef GRAPHPARTITIONING_PARTITIONING_H
#define GRAPHPARTITIONING_PARTITIONING_H

#include "Graph.h"

GraphPtr loadFromFile(const std::string& path);
GraphPtr coarseGraph_s(const GraphPtr& graph);
GraphPtr coarseGraph_p( GraphPtr& graph, int num_threads);
EdgePtrArr sortEdge(const EdgePtrArr& edges);
NodePtrArr sortNodes(const NodePtrArr& nodes);
void partitioning_s(const GraphPtr& g, int requestedPartitions);
void partitioning_p(GraphPtr& g, int requestedPartitions, int num_threads);
void kernighanLin(GraphPtr& g, int num_partitions, std::vector<int>& partitions);
void kernighanLin_p(GraphPtr& g, int num_partitions, std::vector<int> &partitions, int num_colors, std::vector<int> &colors, int num_threads);
//void initial_partitioning_p(std::GraphPtr& g, std::vector<int> &partitions, int num_partitions, int num_threads);
int gain(std::vector<int> &partitions, NodePtr & node_to_move, int to_partition);
unsigned long long calculateCutSize(GraphPtr& g, std::vector<int> &partitions);
void save_to_file(const std::string& path, const GraphPtr& g, const std::vector<int> &partitions, int requestedPartitions);

#endif //GRAPHPARTITIONING_PARTITIONING_H
