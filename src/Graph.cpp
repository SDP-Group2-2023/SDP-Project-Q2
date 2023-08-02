#include "Graph.h"
#include <iostream>

using namespace std;
Node* Graph::add_node(int id, int weight){
    Node* n = new Node(id, weight);
    nodes.push_back(n);
    this->V++;
    return n;
}


Edge* Graph::add_edge(int source, int dest, int distance) {
    Node* node1 = nodes[source];
    Node* node2 = nodes[dest];
    Edge* e = new Edge(distance, node1, node2);
    node1->edges.push_back(e);
    node2->edges.push_back(e);
    this->E++;
    return e;
}

void Graph::print(){
    cout << "Graph with " << this->V << " nodes and " << this->E << " edges" << endl;
    for(int i = 0; i<V; i++){
        Node* n = nodes[i];
        cout << "Node " << n->id << " with weight " << n->weight << endl;
        for(auto & edge : n->edges) {
            int source, dest;
            source = edge->node1->id;
            dest = edge->node2->id;
            if(source != n->id)
                swap(source, dest);
            cout << "\tEdge " << source << " -> "
                 << dest << " with weight "
                 << edge->weight << endl;
        }
    }
}

void Graph::free(){
    for(auto & node : nodes) {
        delete node;
        node = nullptr;
    }
}

void Graph::add_or_sum_edge(int source, int dest, int distance) {
    Node* node1 = nodes[source];
    Node* node2 = nodes[dest];
    for(auto & edge : node1->edges) {
        if(edge->node1 == node2 || edge->node2 == node2){
            edge->weight += distance;
            return;
        }
    }
    add_edge(source, dest, distance);
}