#ifndef GRAPHPARTITIONING_GRAPH_H
#define GRAPHPARTITIONING_GRAPH_H

#include <vector>

using namespace std;

struct Node{
    Node(int id, int weight){
        this->id = id;
        this->weight = weight;
        parent1 = nullptr;
        parent2 = nullptr;
        child = nullptr;
    }
    int id;
    int weight;
    Node *parent1;
    Node *parent2;
    Node *child;
    vector<struct Edge*> edges;
};

struct Edge{
    Edge(int weight, Node* node1, Node* node2){
        this->weight = weight;
        this->node1 = node1;
        this->node2 = node2;
    }
    int weight;
    Node* node1;
    Node* node2;
};



class Graph {
public:
    int V = 0;
    int E = 0;
    vector<Node *> nodes;
    Node *add_node(int id, int weight);
    Edge *add_edge(int source, int dest, int distance);
    void print();
    void free();
    void add_or_sum_edge(int source, int dest, int distance);
    };


#endif //GRAPHPARTITIONING_GRAPH_H
