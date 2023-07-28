#include "Graph.h"
#include <fstream>
#include <string>
#include <thread>
#include <vector>

#define NODES_PER_THREAD 1000000
#define EDGES_PER_THREAD 2500000

using namespace std;

Graph::Graph(string path) {
    ifstream in(path, ios::binary);

    in.read((char *) &this->num_nodes, sizeof(int));
    in.read((char *) &this->num_edges, sizeof(int));

    in.close();

    unsigned int node_readers_num = this->num_nodes / NODES_PER_THREAD + 1;
    unsigned int edge_readers_num = this->num_edges / EDGES_PER_THREAD + 1;

    vector<thread *> nodes_readers;
    vector<thread *> edges_readers;

    adj_matrix = vector<vector<int>>(this->num_nodes, vector<int>(this->num_nodes, 0));

    for (unsigned int i = 0; i < node_readers_num - 1; i++)
        nodes_readers.push_back(new thread(&Graph::read_nodes, this, path, (1 + i * NODES_PER_THREAD) * 2 * sizeof(int), NODES_PER_THREAD));
    nodes_readers.push_back(new thread(&Graph::read_nodes, this, path, (1 + (node_readers_num - 1)) * 2 * sizeof(int), this->num_nodes % NODES_PER_THREAD));

    for (unsigned int i = 0; edge_readers_num - 1; i++)
        edges_readers.push_back(new thread(&Graph::read_edges, this, path, (1 + this->num_nodes) * 2 * sizeof(int) + i * 3 * EDGES_PER_THREAD * sizeof(int), EDGES_PER_THREAD));
    edges_readers.push_back(new thread(&Graph::read_edges, this, path, (1 + this->num_nodes) * 2 * sizeof(int) + (edge_readers_num - 1) * 3 * EDGES_PER_THREAD * sizeof(int),
                                       this->num_edges % EDGES_PER_THREAD));

    for (auto t : nodes_readers) {
        t->join();
        delete t;
    }
    for (auto t : edges_readers) {
        t->join();
        delete t;
    }
}

void Graph::read_nodes(std::string path, unsigned long offset, int to_read) {
    ifstream in(path, ios::binary);
    in.seekg(offset);

    int id, weight;

    for (int i = 0; i < to_read; i++) {
        in.read((char *) &id, sizeof(int));
        in.read((char *) &weight, sizeof(int));

        this->addNodeWeight(id, weight);
    }

    in.close();
}

void Graph::read_edges(std::string path, unsigned long offset, int to_read) {
    ifstream in(path, ios::binary);
    in.seekg(offset);

    int from, to, weight;

    for (int i = 0; i < to_read; i++) {
        in.read((char *) &from, sizeof(int));
        in.read((char *) &to, sizeof(int));
        in.read((char *) &weight, sizeof(int));

        this->addEdge(from, to, weight);
    }

    in.close();
}
