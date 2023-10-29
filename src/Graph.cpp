#include "Graph.h"
#include <iostream>

Node::Node(unsigned int id,unsigned int weight)
: id(id), weight(weight) {}

NodePtrArr Node::get_neighbors() const{
    NodePtrArr neighbors;
    for (const auto &e : edges)
        neighbors.push_back((this->id == e->node1.lock()->id) ? e->node2.lock() : e->node1.lock());
    return neighbors;
}

Edge::Edge(unsigned int weight, const NodePtr & node1, const NodePtr & node2)
: weight(weight), node1(node1), node2(node2){}

NodePtr Graph::add_node(unsigned int id, unsigned int weight){
    auto n = std::make_shared<Node>(id, weight);
    nodes.push_back(n);
    this->node_weight_global += weight;
    return n;
}

NodePtr Graph::add_node_with_index( unsigned int id, unsigned int weight) {
    nodes[id] = std::make_shared<Node>(id, weight);
    this->node_weight_global += weight;
    return nodes[id];
}

EdgePtr Graph::add_edge(unsigned int source, unsigned int dest, unsigned int distance) {
    auto node1        = nodes[source];
    auto node2        = nodes[dest];
    auto e = std::make_shared<Edge>(distance, node1, node2);
    edges.push_back(e);
    node1->edges.push_back(e);
    node2->edges.push_back(e);
    return e;
}

void Graph::add_or_sum_edge(const NodePtr & n1, const NodePtr& n2, unsigned int distance) {
    for (const auto &edge : n1->edges) {
        if (edge->node1.lock() == n2 || edge->node2.lock() == n2) {
            edge->weight += distance;
            return;
        }
    }
    add_edge(n1->id, n2->id, distance);
}

unsigned int Graph::max_node_degree() {
    if (_max_node_degree != 0)
        return _max_node_degree;
    for (const auto& n : nodes) {
        if (_max_node_degree < n->edges.size())
            _max_node_degree = n->edges.size();
    }
    return _max_node_degree;
}

unsigned int Graph::V() const{
    return (int)nodes.size();
}

unsigned long Graph::E() const{
    return (int)edges.size();
}