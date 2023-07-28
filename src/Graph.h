#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <string>

class Graph{
private:
    int num_nodes;
    int num_edges = 0;
    std::map<int, int> nodes_weights;
    std::vector<std::vector<int>> adj_matrix;

    void read_nodes(std::string path, unsigned long offset, int to_read);
    void read_edges(std::string path, unsigned long offset, int to_read);

public:
    Graph(int num_nodes, int num_edges);
    void addEdge(int source, int dest, int weight);
    void addNodeWeight(int node, int weight);
    int getNumNodes() const;
    int getNumEdges() const;
    int getEdge(int source, int dest);
    int getNodeWeight(int node);
    void print();
};

#endif
