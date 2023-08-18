#include "Graph.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <mutex>
#include <thread>
#include <barrier>

using namespace std;

struct m_edge{
    int node1;
    int node2;
    int weight;
};

mutex mtx_e;
void thread_reader(Graph*g,const string &path, unsigned long offset_from_start_nodes,
                   int nodes_to_read,unsigned long offset_from_start_edges, int edges_to_read, barrier<>& b);

Graph* loadFromFile_p(const string& path){
    cout << "Loading graph..." << endl;
    auto start_time = chrono::high_resolution_clock::now();
    ifstream in(path, ios::binary);
    if(!in.is_open())
        throw std::runtime_error("Error opening file");
    auto g = new Graph();
    auto numThreads = thread::hardware_concurrency();
    int num_edges;
    int num_nodes;
    vector<thread> readers;

    in.read((char *) &num_nodes, sizeof(int));
    in.read((char *) &num_edges, sizeof(int));

    cout << "Number of nodes: " << num_nodes << endl;
    cout << "Number of edges: " << num_edges << endl;

    in.close();

    g->nodes.resize(num_nodes);
    g->V = num_nodes;

    auto nodes_per_thread = num_nodes / numThreads;
    auto edges_per_thread = num_edges / numThreads;

    auto offset_nodes = 2 * sizeof(int);
    auto offset_edges = 2 * sizeof(int) * (1 + num_nodes);

    unsigned short reminder_nodes = num_nodes % numThreads;
    unsigned short reminder_edges = num_edges % numThreads;

    int extra_node;
    int extra_edge;

    barrier bar(numThreads);

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

        readers.emplace_back(thread_reader, g, ref(path),
                             offset_nodes, nodes_per_thread + extra_node,
                             offset_edges, edges_per_thread + extra_edge, ref(bar));

        offset_nodes += (nodes_per_thread + extra_node) * sizeof(int) * 2;
        offset_edges += (edges_per_thread + extra_edge) * sizeof(int) * 3;
    }

    for (auto &t : readers)
        t.join();

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Time to load graph: " << duration.count() << "ms" << endl;

    return g;
}


void thread_reader(Graph*g, const string &path, unsigned long offset_from_start_nodes,int nodes_to_read,unsigned long offset_from_start_edges, int edges_to_read, barrier<>& bar){
    ifstream in(path, ios::binary);
    in.seekg(offset_from_start_nodes);

    int n_id;
    int n_weight;
    for (int i = 0; i < nodes_to_read; i++) {
        in.read((char *) &n_id, sizeof(int));
        in.read((char *) &n_weight, sizeof(int));
        g->add_node_with_index(n_id, n_weight);
    }

    vector<m_edge> edges(edges_to_read);
    in.seekg(offset_from_start_edges);
    for (int i = 0; i < edges_to_read; i++) {
        m_edge e{};
        in.read((char *) &e, sizeof(m_edge));
        edges[i] = e;
    }

    bar.arrive_and_wait();


    mtx_e.lock();
    for (auto &e : edges)
        g->add_edge(e.node1, e.node2, e.weight);
    mtx_e.unlock();

    in.close();
}