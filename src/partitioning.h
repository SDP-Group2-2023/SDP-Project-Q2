#ifndef GRAPHPARTITIONING_PARTITIONING_H
#define GRAPHPARTITIONING_PARTITIONING_H

#include "Graph.h"

Graph* loadFromFile(const string& path, int num_threads);
Graph* coarseGraph_s(Graph* graph);
Graph* coarseGraph_p(Graph* graph, int num_threads);
vector<shared_ptr<Edge>> sortEdge(const vector<shared_ptr<Edge>>& edges);
vector<Node*> sortNodes(const vector<Node*>& nodes);
void partitioning_s(Graph*g, int requestedPartitions);
void partitioning_p(Graph*g, int requestedPartitions, int num_threads);
void kernighanLin(Graph* graph, int num_partitions, vector<int>& partitions);


#endif //GRAPHPARTITIONING_PARTITIONING_H
