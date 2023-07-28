#include <vector>
#include <map>

class Graph{
private:
    int num_nodes;
    int num_edges = 0;
    std::map<int, int> nodes_weights;
    std::vector<std::vector<int>> adj_matrix;
public:
    Graph(int num_nodes);
    void addEdge(int source, int dest, int weight);
    void addNodeWeight(int node, int weight);
    int getNumNodes() const;
    int getNumEdges() const;
    int getEdge(int source, int dest) const;
    int getNodeWeight(int node);
    void print();
};