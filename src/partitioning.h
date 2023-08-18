#ifndef GRAPHPARTITIONING_PARTITIONING_H
#define GRAPHPARTITIONING_PARTITIONING_H

#include "Graph.h"

Graph* loadFromFile_s(const string& path);
Graph* loadFromFile_p(const string& path);
vector<Node*> sortNodes(const vector<Node*>& nodes);
vector<Edge*> sortEdge(const vector<Edge*>& edges);
Graph* coarseGraph_s(Graph* graph);
Graph* coarseGraph_p(Graph* graph);
void parallel_partitioning(Graph* graph, int requestedPartitions, int num_threads);
void sequential_partitioning(Graph*g, int requestedPartitions);
void kernighanLin(Graph* graph, int num_partitions, vector<int>& partitions);


#endif //GRAPHPARTITIONING_PARTITIONING_H
