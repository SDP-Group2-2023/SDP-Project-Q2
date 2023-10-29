#include "Graph.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <thread>
#include <barrier>
#include <mutex>

struct m_edge{
    unsigned int node1;
    unsigned int node2;
    unsigned int weight;
};

struct thread_data{
    GraphPtr g;
    unsigned int* filedata;
    unsigned int num_nodes;
    unsigned long num_edges;
    unsigned long num_threads;
};

void thread_reader(const thread_data& data, int start, std::barrier<>& bar, std::mutex& mtx_e){

    auto num_nodes = data.num_nodes;
    auto num_edges = data.num_edges;
    auto g = data.g;
    auto filedata = data.filedata;
    auto step = data.num_threads;

    const unsigned int starting_offeset = 3; //the size of num_nodes + num_edges in the first line of the fie

    unsigned int node_step = step*2;
    unsigned int edge_step = step*3;

    unsigned int n_id;
    unsigned int n_weight;
    unsigned int cursor = start*2 + starting_offeset;
    while(cursor< num_nodes*2 + starting_offeset){
        n_id = filedata[cursor];
        n_weight = filedata[cursor+1];
        g->add_node_with_index(n_id, n_weight);
        cursor += node_step;
    }

    m_edge e_temp{};
    std::vector<m_edge> edges;
    cursor = start * 3 + starting_offeset + num_nodes*2;
    while(cursor< num_nodes*2 + starting_offeset + num_edges*3){
        e_temp.node1 = filedata[cursor];
        e_temp.node2 = filedata[cursor+1];
        e_temp.weight = filedata[cursor+2];
        edges.emplace_back(e_temp);
        cursor += edge_step;
    }

    bar.arrive_and_wait();
    std::scoped_lock<std::mutex> lock(mtx_e);

    for (const auto &e : edges)
        g->add_edge(e.node1, e.node2, e.weight);
}

GraphPtr loadFromFile(const std::string& path) {

    std::cout << "Loading graph from file: " << path << std::endl;

    int num_threads = 8;
    auto start_time = std::chrono::high_resolution_clock::now();
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error("Impossible to open file");
    }

    struct stat fileStat{};
    if (fstat(fd, &fileStat) == -1) {
        throw std::runtime_error("Impossible to get file size");
    }

    auto intData = static_cast<unsigned int*>(mmap(nullptr, fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (intData == MAP_FAILED) {
        throw std::runtime_error("Impossible to map file");
    }

    auto num_nodes = intData[0];
    auto num_edges = static_cast<unsigned long>(intData[2]) << 32 | intData[1];

    auto g = std::make_shared<Graph>();
    g->nodes.resize(num_nodes);

    std::vector<std::thread> readers;
    std::barrier bar(num_threads);
    std::mutex mtx_e;

    thread_data td = {g, intData, num_nodes, num_edges, (unsigned int)num_threads};

    for(int start = 0; start<num_threads; start++){
        readers.emplace_back(thread_reader, std::ref(td), start, std::ref(bar), std::ref(mtx_e));
    }

    for(auto &t: readers)
        t.join();

    munmap(intData, fileStat.st_size);
    close(fd);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Graph loaded in " << duration << " ms" << std::endl;

    return g;
}