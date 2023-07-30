#include "Graph.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

barrier bar { thread::hardware_concurrency() };

Graph::Graph(const string &path) {
    unsigned int numThreads = 2;

    cout << "Number of threads: " << numThreads * 2 << endl;

    int num_edges;
    int num_nodes;
    vector<thread> readers;
    ifstream in(path, ios::binary);
    if (!in.is_open()) {
        cout << "Error opening file" << endl;
        exit(1);
    }

    // cout << "Loading graph" << endl;

    in.read((char *) &num_nodes, sizeof(int));
    in.read((char *) &num_edges, sizeof(int));

    cout << "Number of nodes: " << num_nodes << endl;
    cout << "Number of edges: " << num_edges << endl;

    in.close();

    int nodes_per_thread = num_nodes / numThreads;
    int edges_per_thread = num_edges / numThreads;

    int offset_nodes = 2 * sizeof(int);
    int offset_edges = 2 * sizeof(int) * (1 + num_nodes);

    unsigned short reminder_nodes = num_nodes % numThreads;
    unsigned short reminder_edges = num_edges % numThreads;

    int extra_node;
    int extra_edge;

    for (int i = 0; i < numThreads; i++) {
        if (reminder_nodes > 0) {
            extra_node = 1;
            reminder_nodes--;
        } else
            extra_node = 0;
        if (reminder_edges > 0) {
            extra_edge = 1;
            reminder_edges--;
        } else
            extra_edge = 0;

        readers.emplace_back(&Graph::node_reader, this, path, offset_nodes, nodes_per_thread + extra_node);
        readers.emplace_back(&Graph::edge_reader, this, path, offset_edges, edges_per_thread + extra_edge);

        offset_nodes += (int) (nodes_per_thread + extra_node) * sizeof(int) * 2;
        offset_edges += (int) (edges_per_thread + extra_edge) * sizeof(int) * 3;
    }

    for (auto &t : readers)
        t.join();

    resetMaxIterator();
    // cout << "Graph loaded" << endl;
}

void Graph::node_reader(const string &path, unsigned long offset_from_start_nodes, int nodes_to_read) {
    ifstream in(path, ios::binary);
    in.seekg(offset_from_start_nodes);

    vector<Node> n_buffer(nodes_to_read);

    int n_id;
    int n_weight;

    for (int i = 0; i < nodes_to_read; i++) {
        in.read((char *) &n_id, sizeof(int));
        in.read((char *) &n_weight, sizeof(int));
        // cout << n_id << " " << n_weight << endl;
        n_buffer[i] = Node(n_id, n_weight, -1);
        //this->add_node(n_id, n_weight, -1);
    }

    
    this->mNode.lock();
    for (auto &n : n_buffer)
        this->add_node(n);
    this->mNode.unlock();
    
    // bar.arrive_and_wait();

    // cout << "Thread " << this_thread::get_id() << " finished reading edges" << endl;

    in.close();
}

void Graph::edge_reader(const string &path, unsigned long offset_from_start_edges, int edges_to_read) {
    ifstream in(path, ios::binary);
    in.seekg((int) offset_from_start_edges);

    vector<Edge> e_buffer(edges_to_read);

    int e_source;
    int e_dest;
    int e_weight;

    for (int i = 0; i < edges_to_read; i++) {
        in.read((char *) &e_source, sizeof(int));
        in.read((char *) &e_dest, sizeof(int));
        in.read((char *) &e_weight, sizeof(int));
        
        //this->add_edge(e_source, e_dest, e_weight);
        e_buffer[i] = Edge(e_source, e_dest, e_weight);
        //  cout << e_source << " " << e_dest << " " << e_weight << endl;
    }

    this->mEdge.lock();
    for (auto &e : e_buffer)
        this->add_edge(e);
    this->mEdge.unlock();

    in.close();
}
