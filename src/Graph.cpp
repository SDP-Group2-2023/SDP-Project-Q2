#include "Graph.h"
#include <iostream>

using namespace std;

Node::Node(int id, int weight) {
    this->id     = id;
    this->weight = weight;
    child        = nullptr;
}

vector<Node *> Node::get_neighbors() {
    vector<Node *> neighbors;

    for (auto &e : edges) {
        neighbors.push_back((this == e->node1) ? e->node2 : e->node1);
    }

    return neighbors;
}

Edge::Edge(int weight, Node *node1, Node *node2) {
    this->weight = weight;
    this->node1  = node1;
    this->node2  = node2;
    this->flag   = false;
}

Node *Graph::add_node(int id, int weight) {
    Node *n = new Node(id, weight);
    nodes.push_back(n);
    this->node_weight_global += weight;
    return n;
}

Node *Graph::add_node_with_index(int id, int weight) {
    Node *n = new Node(id, weight);
    nodes[id] = n;
    this->node_weight_global += weight;
    return n;
}

shared_ptr<Edge> Graph::add_edge(int source, int dest, int distance) {
    Node *node1        = nodes[source];
    Node *node2        = nodes[dest];
    shared_ptr<Edge> e = make_shared<Edge>(distance, node1, node2);
    edges.push_back(e);
    node1->edges.push_back(e);
    node2->edges.push_back(e);
    return e;
}

void Graph::print() {
    cout << "Graph with " << V() << " nodes and " << E() << " edges" << endl;
    for (int i = 0; i < V(); i++) {
        Node *n = nodes[i];
        cout << "Node " << n->id << " with weight " << n->weight << endl;
        for (auto &edge : n->edges) {
            int source, dest;
            source = edge->node1->id;
            dest   = edge->node2->id;
            if (source != n->id)
                swap(source, dest);
            cout << "\tEdge " << source << " -> " << dest << " with weight " << edge->weight << endl;
        }
    }
}

Graph::~Graph() {
    for (auto &node : nodes) {
        for (auto &edge : node->edges) {
            edge = nullptr;
        }
        delete node;
        node = nullptr;
    }
}

void Graph::add_or_sum_edge(Node *n1, Node *n2, int distance) {
    for (auto &edge : n1->edges) {
        if (edge->node1 == n2 || edge->node2 == n2) {
            edge->weight += distance;
            return;
        }
    }
    add_edge(n1->id, n2->id, distance);
}

int Graph::max_node_degree() {
    if (_max_node_degree != 0)
        return _max_node_degree;
    for (Node *n : nodes) {
        if (_max_node_degree < n->edges.size())
            _max_node_degree = n->edges.size();
    }

    return _max_node_degree;
}

int Graph::V(){
    return (int)nodes.size();
}

int Graph::E(){
    return (int)edges.size();
}