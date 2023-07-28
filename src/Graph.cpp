#include "Graph.h"
#include <vector>
#include <iostream>

using namespace std;
Graph::Graph(int num_nodes):
    num_nodes(num_nodes),
    adj_matrix(vector<vector<int>>(num_nodes, vector<int>(num_nodes, 0)))
    {}

void Graph::addEdge(int source, int dest, int weight) {
    adj_matrix[source][dest] = weight;
    adj_matrix[dest][source] = weight;
    num_edges++;
}

void Graph::addNodeWeight(int node, int weight) {
    nodes_weights[node] = weight;
}

int Graph::getNumNodes() const {
    return num_nodes;
}

int Graph::getNumEdges() const {
    return num_edges;
}

int Graph::getEdge(int source, int dest) const{
    return adj_matrix[source][dest];
}

void Graph::print(){
    cout << "num_nodes: " << num_nodes << endl;
    cout << "num_edges: " << num_edges << endl;
    for(auto & nodes_weight : nodes_weights){
        cout << nodes_weight.first << " " << nodes_weight.second << endl;
    }
    for(int i = 0; i<num_nodes; i++){
        cout << i << ": " << endl;
        for(int j = 0; j<num_nodes; j++){
            if(adj_matrix[i][j] != 0)
                cout << "\tdest:" <<j << " weight:"<< adj_matrix[i][j] << endl;
        }
        cout << endl;
    }
}

int Graph::getNodeWeight(int node) {
    return nodes_weights[node];
}