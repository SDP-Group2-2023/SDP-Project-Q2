#include "Graph.h"
#include <iostream>

Node::Node(unsigned int id,unsigned int weight) : id(id), weight(weight), child(nullptr) {}

std::vector<NodePtr> Node::get_neighbors() const{
    std::vector<NodePtr> neighbors;

    for (auto &e : edges) {
        neighbors.push_back((this->id == e->node1->id) ? e->node2 : e->node1);
    }

    return neighbors;
}

Edge::Edge(unsigned int weight, NodePtr & node1, NodePtr & node2)
: weight(weight), node1(node1), node2(node2), flag(false) {}

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

/*
void Graph::print() {
    std::cout << "Graph with " << V() << " nodes and " << E() << " edges" << std::endl;
    for (int i = 0; i < V(); i++) {
        auto n = nodes[i];
        std::cout << "Node " << n->id << " with weight " << n->weight << std::endl;
        for (auto &edge : n->edges) {
            unsigned int source, dest;
            source = edge->node1->id;
            dest   = edge->node2->id;
            if (source != n->id)
                std::swap(source, dest);
            std::cout << "\tEdge " << source << " -> " << dest << " with weight " << edge->weight << std::endl;
        }
    }
}
 */

/*
Graph::~Graph() {
    for (auto &node : nodes) {
        for (auto &edge : node->edges) {
            edge = nullptr;
        }
        node = nullptr;
    }
}
 */

void Graph::add_or_sum_edge(const NodePtr & n1, const NodePtr& n2, unsigned int distance) {
    for (auto &edge : n1->edges) {
        if (edge->node1 == n2 || edge->node2 == n2) {
            edge->weight += distance;
            return;
        }
    }
    add_edge(n1->id, n2->id, distance);
}

unsigned int Graph::max_node_degree() {
    if (_max_node_degree != 0)
        return _max_node_degree;
    for (auto& n : nodes) {
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