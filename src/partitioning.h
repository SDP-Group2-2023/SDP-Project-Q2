#ifndef GRAPHPARTITIONING_PARTITIONING_H
#define GRAPHPARTITIONING_PARTITIONING_H

#include "Graph.h"

std::shared_ptr<Graph> loadFromFile(const std::string& path);
std::shared_ptr<Graph> coarseGraph_s(std::shared_ptr<Graph> &graph);
std::shared_ptr<Graph> coarseGraph_p(std::shared_ptr<Graph>& graph, int num_threads);
std::vector<std::shared_ptr<Edge>> sortEdge(const std::vector<std::shared_ptr<Edge>>& edges);
std::vector<Node*> sortNodes(const std::vector<Node*>& nodes);
void partitioning_s(std::shared_ptr<Graph>& g, int requestedPartitions);
void partitioning_p(std::shared_ptr<Graph>& g, int requestedPartitions, int num_threads);
void kernighanLin(std::shared_ptr<Graph>& g, int num_partitions, std::vector<int>& partitions);
void kernighanLin_p(std::shared_ptr<Graph>& g, int num_partitions, std::vector<int> &partitions, int num_colors, std::vector<int> &colors, int num_threads);
//void initial_partitioning_p(std::shared_ptr<Graph>& g, std::vector<int> &partitions, int num_partitions, int num_threads);
int gain(std::vector<int> &partitions, Node *node_to_move, int to_partition);
unsigned long long calculateCutSize(std::shared_ptr<Graph>& g, std::vector<int> &partitions);
void save_to_file(std::string path, std::shared_ptr<Graph>& g, const std::vector<int> &partitions, int requestedPartitions);

#endif //GRAPHPARTITIONING_PARTITIONING_H
