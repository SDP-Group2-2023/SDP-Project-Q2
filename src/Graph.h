#ifndef GRAPHPARTITIONING_GRAPH_H
#define GRAPHPARTITIONING_GRAPH_H

#include <vector>
#include <iostream>
#include <memory>

using namespace std;

struct Node{
    Node(int id, int weight){
        this->id = id;
        this->weight = weight;
        child = nullptr;
    }
    int id;
    int weight;
    Node *child;
    vector<shared_ptr<struct Edge>> edges;
};

struct Edge{
    Edge(int weight, Node* node1, Node* node2){
        this->weight = weight;
        this->node1 = node1;
        this->node2 = node2;
        this->flag = false;
    }
    int weight;
    Node* node1;
    Node* node2;
    bool flag ;
};



class Graph {
public:
    int V = 0;
    int E = 0;
    vector<Node *> nodes;
    vector<shared_ptr<Edge>> edges;
    Node *add_node(int id, int weight);
    shared_ptr<Edge> add_edge(int source, int dest, int distance);
    void print();
    void add_or_sum_edge(Node*n1, Node*n2, int distance);
    ~Graph();
 };


#endif //GRAPHPARTITIONING_GRAPH_H
