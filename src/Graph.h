#ifndef GRAPHPARTITIONING_GRAPH_H
#define GRAPHPARTITIONING_GRAPH_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

typedef std::shared_ptr<struct Node> NodePtr;
typedef std::shared_ptr<struct Edge> EdgePtr;
typedef std::shared_ptr<struct Graph> GraphPtr;

struct Node {
    Node(unsigned int id, unsigned int weight);
    unsigned int id;
    unsigned int weight;
    NodePtr child;
    std::vector<EdgePtr> edges;
    std::vector<NodePtr> get_neighbors() const;
    //~Node(){std::cout << "Node destructor called" << std::endl;}
};

struct Edge {
    Edge(unsigned int weight, NodePtr& node1, NodePtr& node2);
    unsigned int weight;
    NodePtr node1;
    NodePtr node2;
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
    std::vector<NodePtr> nodes;
    std::vector<EdgePtr> edges;
    NodePtr add_node(unsigned int id, unsigned int weight);
    NodePtr add_node_with_index(unsigned int id, unsigned int weight);
    EdgePtr add_edge(unsigned int source, unsigned int dest, unsigned int distance);
    //void print();
    void add_or_sum_edge(const NodePtr& n1,const NodePtr& n2, unsigned int distance);
    unsigned int max_node_degree();
    ~Graph(){std::cout << "Graph destructor called" << std::endl;}
};

#endif    // GRAPHPARTITIONING_GRAPH_H
