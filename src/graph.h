#include <vector>

using namespace std;
class Graph{
private:
    int num_nodes;
    int num_edges;
    vector<vector<int>> adj_matrix;
public:
    Graph(int num_nodes, int num_edges);
    void addEdge(int u, int v);
};

Graph::Graph(int num_nodes, int num_edges) {
    this->num_nodes = num_nodes;
    this->num_edges = num_edges;
    adj_matrix = vector<vector<int>>(num_nodes, vector<int>(num_nodes, 0));
}

void Graph::addEdge(int u, int v) {
    adj_matrix[u][v] = 1;
    adj_matrix[v][u] = 1;
}
