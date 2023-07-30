#include "Graph.h"
#include <iostream>

using namespace std;

bool Edge::operator<(const Edge &a) const {
        return (this->weight > a.weight) || (this->weight == a.weight && (this->source > a.source || (this->source == a.source && this->dest > a.dest)));
    }

    bool Edge::operator>(const Edge &a) const {
        return (this->weight < a.weight) || (this->weight == a.weight && (this->source < a.source || (this->source == a.source && this->dest < a.dest)));
    }

    bool Edge::operator==(const Edge &c) const {
        return this->source == c.source && this->dest == c.dest;
    }

Graph::Graph() = default;

void Graph::add_node(int id, int weight, int partition) {
    Node nNode { id, weight, partition };
    nodes.emplace_back(nNode);
}

void Graph::add_node(const Node &n_node) {
    nodes.emplace_back(n_node);
}

void Graph::add_edge(int source, int dest, int weight) {
    if (source < dest)
        swap(source, dest);
    Edge nEdge { source, dest, weight };

    if (edges_look_up[source].contains(dest))
        return;

    edges_look_up[source][dest] = weight;

    edges.insert(nEdge);
}

void Graph::add_edge(Edge n_edge) {
    if(n_edge.source < n_edge.dest)
        swap(n_edge.source, n_edge.dest);

    if (edges_look_up[n_edge.source].contains(n_edge.dest))
        return;

    edges_look_up[n_edge.source][n_edge.dest] = n_edge.weight;
    edges.insert(n_edge);
}

Node Graph::get_node(int id) {
    return nodes[id];
}

Edge Graph::get_edge(int source, int dest) {
    if (source < dest)
        swap(source, dest);

    if(edges_look_up[source].contains(dest))
        return Edge(source, dest, edges_look_up[source][dest]);

    throw runtime_error("Edge not found");
}

int Graph::get_num_edges() {
    return (int) edges.size();
}

int Graph::get_num_nodes() {
    return (int) nodes.size();
}

void Graph::print() {
    for (int i = 0; i < nodes.size(); i++) {
        Node node = nodes[i];
        cout << "n " << node.id << " " << node.weight << " " << node.partition << endl;
    }

    int num_nodes = (int) nodes.size();

    set<Edge>::iterator it;
    for (it = edges.begin(); it != edges.end(); ++it) {
        Edge tempEdge = *it;
        cout << "a " << tempEdge.source << " " << tempEdge.dest << " " << tempEdge.weight << endl;

        if (tempEdge.source > num_nodes - 1 || tempEdge.dest > num_nodes - 1)
            cout << "Edge source or destination out of bounds" << endl;
    }
}

void Graph::set_node_partition(int id, int partition) {
    nodes[id].partition = partition;
}

void Graph::increase_edge(int source, int dest, int weight_increment) {
    Edge e = this->get_edge(source, dest);
    this->edges.erase(e);
    e.weight += weight_increment;
    this->edges.insert(e);
}

