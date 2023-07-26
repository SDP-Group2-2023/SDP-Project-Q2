#include "Graph.h"
#include <vector>

Graph::Graph(int num_nodes, int num_edges) {
    this->num_nodes = num_nodes;
    this->num_edges = num_edges;
    adj_matrix = std::vector<std::vector<int>>(num_nodes, std::vector<int>(num_nodes, 0));
}

void Graph::addEdge(int u, int v) {
    adj_matrix[u][v] = 1;
    adj_matrix[v][u] = 1;
}