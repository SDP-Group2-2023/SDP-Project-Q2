#ifndef GRAPHPARTITIONING_PARTITIONING_H
#define GRAPHPARTITIONING_PARTITIONING_H

#include "Graph.h"
#include <thread>

GraphPtr loadFromFile(const std::string& path, unsigned int num_threads = std::thread::hardware_concurrency(), unsigned int myStep = 10);
GraphPtr coarseGraph_s(const GraphPtr& graph);
GraphPtr coarseGraph_p( GraphPtr& graph, int num_threads);
EdgePtrArr sortEdge(const EdgePtrArr& edges);
NodePtrArr sortNodes(const NodePtrArr& nodes);
void partitioning_s(const GraphPtr& g, int requestedPartitions);
void partitioning_p(const GraphPtr& g, int requestedPartitions, int num_threads);
void kernighanLin(const GraphPtr& g, int num_partitions, std::vector<unsigned int>& partitions);
void kernighanLin_p(const GraphPtr& g, int num_partitions, std::vector<unsigned int> &partitions, unsigned int num_colors, std::vector<int> &colors, int num_threads);
//void initial_partitioning_p(std::GraphPtr& g, std::vector<int> &partitions, int num_partitions, int num_threads);
int countPartitionWeight(const GraphPtr &graph, int partition, std::vector<unsigned int> &partitions);
int gain(std::vector<unsigned int> &partitions, const NodePtr & node_to_move, int to_partition);
unsigned long long calculateCutSize(const GraphPtr& g, std::vector<unsigned int> &partitions);
void save_to_file(const std::string& path, const GraphPtr& g, const std::vector<unsigned int> &partitions, int requestedPartitions);

#endif //GRAPHPARTITIONING_PARTITIONING_H
