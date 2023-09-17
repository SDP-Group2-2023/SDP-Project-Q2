#ifndef GRAPHPARTITIONING_GRAPH_H
#define GRAPHPARTITIONING_GRAPH_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

struct Node {
    Node(int id, int weight);
    int id;
    int weight;
    Node *child;
    vector<shared_ptr<struct Edge>> edges;
    vector<Node *> get_neighbors();
};

struct Edge {
    Edge(int weight, Node *node1, Node *node2);
    int weight;
    Node *node1;
    Node *node2;
    bool flag;
};

struct Graph {
    int V();                    // nodes_num
    int E();                    // edges_num
    int node_weight_global = 0;
    int _max_node_degree   = 0;
    int num_colours        = 0;
    vector<int> colours;
    vector<int> partitions_size;
    vector<Node *> nodes;
    vector<shared_ptr<Edge>> edges;
    Node *add_node(int id, int weight);
    Node *add_node_with_index(int id, int weight);
    shared_ptr<Edge> add_edge(int source, int dest, int distance);
    void print();
    void add_or_sum_edge(Node *n1, Node *n2, int distance);
    int max_node_degree();
    ~Graph();
};

#endif    // GRAPHPARTITIONING_GRAPH_H
