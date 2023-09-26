#ifndef GRAPHPARTITIONING_GRAPH_H
#define GRAPHPARTITIONING_GRAPH_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

struct Node {
    Node(unsigned int id, unsigned int weight);
    unsigned int id;
    unsigned int weight;
    Node *child;
    vector<shared_ptr<struct Edge>> edges;
    vector<Node *> get_neighbors();
};

struct Edge {
    Edge(unsigned int weight, Node *node1, Node *node2);
    unsigned int weight;
    Node *node1;
    Node *node2;
    bool flag;
};

struct Graph {
    unsigned int V() const;                    // nodes_num
    unsigned long E();                    // edges_num
    unsigned int node_weight_global = 0;
    unsigned int _max_node_degree   = 0;
    unsigned int num_colours        = 0;
    vector<int> colours;
    vector<int> partitions_size;
    vector<Node *> nodes;
    vector<shared_ptr<Edge>> edges;
    Node *add_node(unsigned int id, unsigned int weight);
    Node *add_node_with_index(unsigned int id, unsigned int weight);
    shared_ptr<Edge> add_edge(unsigned int source, unsigned int dest, unsigned int distance);
    void print();
    void add_or_sum_edge(Node *n1, Node *n2, int distance);
    int max_node_degree();
    ~Graph();
};

#endif    // GRAPHPARTITIONING_GRAPH_H
