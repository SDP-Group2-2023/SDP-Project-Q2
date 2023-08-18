#ifndef GRAPHPARTITIONING_GRAPH_H
#define GRAPHPARTITIONING_GRAPH_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <set>

using namespace std;

struct Node {
    Node(int id, int weight);
    int id;
    int weight;
    Node *child;
    vector<struct Edge*> edges;
    vector<Node *> get_neighbors();
};

struct Edge {
    Edge(int weight, Node *node1, Node *node2);
    int weight;
    Node *node1;
    Node *node2;
    bool flag;
};

struct comparator {
    bool operator()(const Edge*e1, const Edge*e2);
};

struct Graph {
    int V                  = 0;    // nodes_num
    int E                  = 0;    // edges_num
    int node_weight_global = 0;
    int _max_node_degree   = 0;
    vector<int> partitions_size;
    vector<Node *> nodes;
    //set<Edge*, comparator> edges;
    vector<Edge*> edges;
    Node *add_node(int id, int weight);
    Node*add_node_with_index(int id, int weight);
    Edge* add_edge(int source, int dest, int distance);
    void print();
    void add_or_sum_edge(Node *n1, Node *n2, int distance);
    int max_node_degree();
    ~Graph();
};

#endif    // GRAPHPARTITIONING_GRAPH_H
