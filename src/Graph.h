#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;
class Graph{
private:
    int num_nodes;
    int num_edges = 0;
    unordered_map<int, int> nodes_weights;
    unordered_map<int, unordered_map<int, int>> adjacencyMap;
    void read_nodes(const string& path, unsigned long offset, int to_read);
    void read_edges(const string& path, unsigned long offset, int to_read);

public:
    Graph(int num_nodes);
    Graph(const string& path);
    void addEdge(int source, int dest, int weight);
    void addNodeWeight(int node, int weight);
    int getNumNodes() const;
    int getNumEdges() const;
    int getNodeWeight(int node);
    void print();
    int getEdge(int source, int dest) const;
};

#endif
