#include "Graph.h"
#include <iostream>

using namespace std;
Graph::Graph(int num_nodes):
    num_nodes(num_nodes)
    {}

void Graph::addEdge(int source, int dest, int weight) {
    if(source < dest) { // swap so the source is the greatest num
        source ^= dest;
        dest ^= source;
        source ^= dest;
    }

    adjacencyMap[make_pair(source, dest)] = weight;
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
    if(source < dest) { // swap so the source is the greatest num
        source ^= dest;
        dest ^= source;
        source ^= dest;
    }

    if(adjacencyMap.find(make_pair(source, dest)) == adjacencyMap.end())
        return 0;
    return adjacencyMap.at(make_pair(source, dest));
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
            int weight = this->getEdge(i, j);
            if (weight != 0)
                cout << "\tdest:" <<j << " weight:"<< weight << endl;
        }
        cout << endl;
    }
}

int Graph::getNodeWeight(int node) {
    return nodes_weights[node];
}