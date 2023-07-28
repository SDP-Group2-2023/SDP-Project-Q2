#include "Graph.h"
#include <iostream>

using namespace std;
Graph::Graph(int num_nodes):
    num_nodes(num_nodes)
    {}

void Graph::addEdge(int source, int dest, int weight) {
    adjacencyMap[source][dest] = weight;
    adjacencyMap[dest][source] = weight;
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
    if(adjacencyMap.find(source) == adjacencyMap.end())
        return 0;
    if(adjacencyMap.at(source).find(dest) == adjacencyMap.at(source).end())
        return 0;
    return adjacencyMap.at(source).at(dest);
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
            if(this->getEdge(i, j) != 0)
                cout << "\tdest:" <<j << " weight:"<< adjacencyMap[i][j] << endl;
        }
        cout << endl;
    }
}

int Graph::getNodeWeight(int node) {
    return nodes_weights[node];
}