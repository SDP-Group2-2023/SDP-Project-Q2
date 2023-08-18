#include "Graph.h"
#include <iostream>

using namespace std;

/**
 * @brief operator to define order of edges in a set
 * @param e1 first edge
 * @param e2 second edge
 * @return boolean value
 */
bool comparator::operator()(const Edge* e1, const Edge*e2) {
    int e1_node1_id = e1->node1->id;
    int e1_node2_id = e1->node2->id;
    if(e1_node1_id > e1_node2_id)
        swap(e1_node1_id, e1_node2_id);

    int e2_node1_id = e2->node1->id;
    int e2_node2_id = e2->node2->id;
    if(e2_node1_id > e2_node2_id)
        swap(e2_node1_id, e2_node2_id);

    if(e1->weight == e2->weight) {
        if(e1_node1_id == e2_node1_id)
            return e1_node2_id < e2_node2_id;
        return e1_node1_id < e2_node1_id;
    }

    return e1->weight > e2->weight;
}

/**
 * @brief Node constructor
 * @param id
 * @param weight
 */
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

/**
 * @brief Edge constructor
 * @param weight
 * @param node1
 * @param node2
 */
Edge::Edge(int weight, Node *node1, Node *node2) {
    this->weight = weight;
    this->node1  = node1;
    this->node2  = node2;
    this->flag   = false;
}

/**
 * @brief create and add a node to the graph
 * @param id
 * @param weight
 * @return created node
 */
Node *Graph::add_node(int id, int weight) {
    Node *n = new Node(id, weight);
    nodes.push_back(n);
    this->node_weight_global += weight;
    this->V++;
    return n;
}

Node*Graph::add_node_with_index(int id, int weight) {
    Node *n = new Node(id, weight);
    nodes[id] = n;
    return n;
}

/**
 * @brief create and add an edge to the graph
 * @param source
 * @param dest
 * @param distance
 * @return created edge
 */
Edge* Graph::add_edge(int source, int dest, int distance) {
    Node *node1        = nodes[source];
    Node *node2        = nodes[dest];
    Edge* e = new Edge(distance, node1, node2);
    //edges.insert(e);
    edges.emplace_back(e);
    node1->edges.push_back(e);
    node2->edges.push_back(e);
    this->E++;
    return e;
}

void Graph::print() {
    cout << "Graph with " << this->V << " nodes and " << this->E << " edges" << endl;
    for (int i = 0; i < V; i++) {
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

/**
 * @brief Graph destructor. Deallocates all nodes and edges
 */
Graph::~Graph() {
    for (auto &node : nodes) {
        for (auto &edge : node->edges) {
            edge = nullptr;
        }
        delete node;
        node = nullptr;
    }
    for(auto &edge : edges) {
        delete edge;
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
            _max_node_degree = (int)n->edges.size();
    }

    return _max_node_degree;
}
