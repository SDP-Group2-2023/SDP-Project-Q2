#ifndef GRAPHPARTITIONING_GRAPH_H
#define GRAPHPARTITIONING_GRAPH_H

#include <vector>
#include <unordered_map>

using namespace std;
struct Node;
struct Edge{
    Edge(int weight, Node* node1, Node* node2){
        this->weight = weight;
        this->node1 = node1;
        this->node2 = node2;
    }
    int weight;
    Node* node1;
    Node* node2;

    bool operator==(const Edge& other) const{
        return (node1 == other.node1 && node2 == other.node2) ||
               (node1 == other.node2 && node2 == other.node1);
    }
};

struct Node{
    Node(int id, int weight){
        this->id = id;
        this->weight = weight;
        this->parent1 = nullptr;
        this->parent2 = nullptr;
        this->child = nullptr;
    }
    int id;
    int weight;
    Node *parent1;
    Node *parent2;
    Node *child;
    vector<Edge*> edges;

    bool operator<(const Node& other) const{
        return weight > other.weight;
    }


};

class Graph {
public:
    int V = 0;
    int E = 0;
    vector<Node *> nodes;
    //vector<Edge *> edges;
    //unordered_map<int, unordered_map<int, Edge*>> edge_map;
    Node *add_node(int id, int weight);
    Node *add_node(Node*node);
    Edge *add_edge(int source, int dest, int distance);
    void print();
    void free();
    void add_or_sum_edge(int source, int dest, int distance);
    };


#endif //GRAPHPARTITIONING_GRAPH_H
