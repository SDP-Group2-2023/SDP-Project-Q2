#include "Graph.h"

#include <iostream>
#include <fstream>

using namespace std;
Graph* loadFromFile(const string& path){
    auto g = new Graph();
    ifstream file(path, ios::binary);
    if(!file.is_open()){
        cout << "Error opening file" << endl;
        exit(1);
    }
    int num_nodes;
    int num_edges;
    file.read((char*)&num_nodes, sizeof(int));
    file.read((char*)&num_edges, sizeof(int));

    int id, node_weight;
    for(int i= 0; i<num_nodes; i++){
        file.read((char*)&id, sizeof(int));
        file.read((char*)&node_weight, sizeof(int));
        g->add_node(id, node_weight);
    }

    int source, dest, distance;
    for(int i = 0; i<num_edges; i++){
        file.read((char*)&source, sizeof(int));
        file.read((char*)&dest, sizeof(int));
        file.read((char*)&distance, sizeof(int));
        g->add_edge(source, dest, distance);
    }

    return g;
}