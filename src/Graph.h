#include <vector>

class Graph{
private:
    int num_nodes;
    int num_edges;
    std::vector<std::vector<int>> adj_matrix;
public:
    Graph(int num_nodes, int num_edges);
    void addEdge(int u, int v);
};