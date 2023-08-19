#include "Graph.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <thread>
#include <barrier>
#include <mutex>

using namespace std;

struct m_edge{
    int node1;
    int node2;
    int weight;
};
void thread_reader(Graph*g, const int*filedata, int num_nodes, int num_edges, int start, int step, barrier<>& bar, mutex& mtx_e);

Graph* loadFromFile_mmap(const string&path, int num_threads) {
    auto start_time = chrono::high_resolution_clock::now();
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error("Impossible to open file");
    }

    struct stat fileStat{};
    if (fstat(fd, &fileStat) == -1) {
        throw std::runtime_error("Impossible to get file size");
    }

    int* intData = static_cast<int*>(mmap(nullptr, fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (intData == MAP_FAILED) {
        throw std::runtime_error("Impossible to map file");
    }

    int num_nodes = intData[0];
    int num_edges = intData[1];

    auto* g = new Graph();
    g->nodes.resize(num_nodes);
    g->V = num_nodes;

    vector<thread> readers(num_threads);
    barrier bar(num_threads);
    mutex mtx_e;
    for(int i = 0; i<num_threads; i++){
        readers[i] = thread(thread_reader, g, intData, num_nodes, num_edges, i, num_threads, ref(bar), ref(mtx_e));
    }

    for(auto &t: readers)
        t.join();

    munmap(intData, fileStat.st_size);
    close(fd);

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cout << "Graph loaded in " << duration << " ms" << endl;

    return g;
}



void thread_reader(Graph*g, const int*filedata, int num_nodes, int num_edges, int start, int step, barrier<>& bar, mutex& mtx_e){
    int n_id;
    int n_weight;
    int cursor = start + 2;
    while(cursor< num_nodes*2 + 2){
        n_id = filedata[cursor];
        n_weight = filedata[cursor+1];
        g->add_node_with_index(n_id, n_weight);
        cursor += step;
    }

    m_edge e_temp{};
    vector<m_edge> edges;
    cursor = start + 2 + num_nodes*2;
    while(cursor< num_nodes*2 + 2 + num_edges*3){
        e_temp.node1 = filedata[cursor];
        e_temp.node2 = filedata[cursor+1];
        e_temp.weight = filedata[cursor+2];
        edges.emplace_back(e_temp);
        cursor += step;
    }

    bar.arrive_and_wait();

    mtx_e.lock();
    for (auto &e : edges)
        g->add_edge(e.node1, e.node2, e.weight);
    mtx_e.unlock();
}