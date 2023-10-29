#ifndef GRAPHPARTITIONING_GRAPH_H
#define GRAPHPARTITIONING_GRAPH_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using NodePtr = std::shared_ptr<struct Node> ;
using EdgePtr = std::shared_ptr<struct Edge> ;
using GraphPtr =  std::shared_ptr<struct Graph>;
using EdgePtrArr = std::vector<EdgePtr>;
using NodePtrArr = std::vector<NodePtr>;

struct Node {
    Node(unsigned int id, unsigned int weight);
    unsigned int id;
    unsigned int weight;
    NodePtr child = nullptr;
    EdgePtrArr edges;
    NodePtrArr get_neighbors() const;
};

struct Edge {
    Edge(unsigned int weight,const NodePtr& node1, const NodePtr& node2);
    unsigned int weight;
    std::weak_ptr<Node> node1;
    std::weak_ptr<Node> node2;
};

struct Graph {
    unsigned int V() const;                    // nodes_num
    unsigned long E() const;                    // edges_num
    unsigned int node_weight_global = 0;
    unsigned int _max_node_degree   = 0;
    unsigned int num_colours        = 0;
    std::vector<int> colours;
    std::vector<int> partitions_size;
    NodePtrArr nodes;
    EdgePtrArr edges;
    NodePtr add_node(unsigned int id, unsigned int weight);
    NodePtr add_node_with_index(unsigned int id, unsigned int weight);
    EdgePtr add_edge(unsigned int source, unsigned int dest, unsigned int distance);
    //void print();
    void add_or_sum_edge(const NodePtr& n1,const NodePtr& n2, unsigned int distance);
    unsigned int max_node_degree();
};

#endif    // GRAPHPARTITIONING_GRAPH_H
