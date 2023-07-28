#include "Graph.h"
#include <fstream>
#include <thread>
#include <vector>
#include <iostream>

using namespace std;

barrier bar{thread::hardware_concurrency()};

Graph::Graph(const string& path) {

    unsigned int numThreads = thread::hardware_concurrency();

    cout << "Number of threads: " << numThreads << endl;

    int num_edges;
    int num_nodes;
    vector<thread> readers;
    ifstream in(path, ios::binary);
    if (!in.is_open()){
            cout << "Error opening file" << endl;
            exit(1);
    }

    //cout << "Loading graph" << endl;

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
        if(reminder_nodes > 0){
            extra_node = 1;
            reminder_nodes--;
        }
        else extra_node = 0;
        if(reminder_edges > 0){
            extra_edge = 1;
            reminder_edges--;
        }
        else extra_edge = 0;

        readers.emplace_back(&Graph::thread_reader, this, path,
            offset_nodes, nodes_per_thread + extra_node, offset_edges, edges_per_thread + extra_edge);

        offset_nodes += (nodes_per_thread + extra_node) * sizeof(int) * 2;
        offset_edges += (edges_per_thread + extra_edge) * sizeof(int) * 3;
    }

    for (auto &t : readers)
        t.join();

    // cout << "Graph loaded" << endl;
}

void Graph::thread_reader(const string &path, unsigned long offset_from_start_nodes, 
int nodes_to_read,unsigned long offset_from_start_edges, int edges_to_read) {
    ifstream in(path, ios::binary);
    in.seekg(offset_from_start_nodes);

    int n_id;
    int n_weight;

    for (int i = 0; i < nodes_to_read; i++) {
        in.read((char *) &n_id, sizeof(int));
        in.read((char *) &n_weight, sizeof(int));
        //cout << n_id << " " << n_weight << endl;

        this->add_node(n_id, n_weight);
    }

    //cout << "Thread " << this_thread::get_id() << " finished reading nodes" << endl;
    bar.arrive_and_wait();

    in.seekg(offset_from_start_edges);

    //cout << "Thread " << this_thread::get_id() << " started reading edges" << endl;

    int e_source;
    int e_dest;
    int e_weight;

    for (int i = 0; i < edges_to_read; i++) {
        in.read((char *) &e_source, sizeof(int));
        in.read((char *) &e_dest, sizeof(int));
        in.read((char *) &e_weight, sizeof(int));

        this->add_edge(e_source, e_dest, e_weight);
        //cout << e_source << " " << e_dest << " " << e_weight << endl;

    }

    //cout << "Thread " << this_thread::get_id() << " finished reading edges" << endl;

    in.close();
}