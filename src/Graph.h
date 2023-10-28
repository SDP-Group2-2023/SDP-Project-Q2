#ifndef GRAPHPARTITIONING_GRAPH_H
#define GRAPHPARTITIONING_GRAPH_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

struct Node {
    Node(unsigned int id, unsigned int weight);
    unsigned int id;
    unsigned int weight;
    std::shared_ptr<Node> child;
    std::vector<std::shared_ptr<struct Edge>> edges;
    std::vector<std::shared_ptr<Node>> get_neighbors() const;
};

struct Edge {
    Edge(unsigned int weight, std::shared_ptr<Node>& node1, std::shared_ptr<Node>& node2);
    unsigned int weight;
    std::shared_ptr<Node> node1;
    std::shared_ptr<Node> node2;
    bool flag;
};

struct Graph {
    unsigned int V() const;                    // nodes_num
    unsigned long E() const;                    // edges_num
    unsigned int node_weight_global = 0;
    unsigned int _max_node_degree   = 0;
    unsigned int num_colours        = 0;
    std::vector<int> colours;
    std::vector<int> partitions_size;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Edge>> edges;
    std::shared_ptr<Node> add_node(unsigned int id, unsigned int weight);
    std::shared_ptr<Node> add_node_with_index(unsigned int id, unsigned int weight);
    std::shared_ptr<Edge> add_edge(unsigned int source, unsigned int dest, unsigned int distance);
    //void print();
    void add_or_sum_edge(std::shared_ptr<Node>& n1, std::shared_ptr<Node>& n2, unsigned int distance);
    unsigned int max_node_degree();
    //~Graph();
};

#endif    // GRAPHPARTITIONING_GRAPH_H
