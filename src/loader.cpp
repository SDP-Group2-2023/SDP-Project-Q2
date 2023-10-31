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
    m_edge(unsigned int n1, unsigned int n2, unsigned int w) : node1(n1), node2(n2), weight(w) {};
};

struct thread_data{
    GraphPtr g;
    unsigned int* file_data;
    unsigned int num_nodes;
    unsigned long num_edges;
    unsigned long step;
};

const unsigned int starting_offset = 3; //the size of num_nodes + num_edges in the first line of the file

void thread_reader(const thread_data& data, int start, std::barrier<>& bar, std::mutex& mtx_e){

    //si moltiplica lo step per 2 perché un'informazione su un nodo è composta da due celle
    unsigned int node_step = data.step*2;

    unsigned int cursor = start*2 + starting_offset;
    while(cursor< data.num_nodes*2 + starting_offset){
        data.g->add_node_with_index(data.file_data[cursor], data.file_data[cursor+1]);
        cursor += node_step;
    }


    //si moltiplica lo step per 3 perché un'informazione su un arco è composta da tre celle
    unsigned int edge_step = data.step*3;

    std::vector<m_edge> edges;
    cursor = start * 3 + starting_offset + data.num_nodes*2;
    while(cursor< data.num_nodes*2 + starting_offset + data.num_edges*3){
        edges.emplace_back(data.file_data[cursor], data.file_data[cursor+1], data.file_data[cursor+2]);
        cursor += edge_step;
    }

    bar.arrive_and_wait();
    std::scoped_lock<std::mutex> lock(mtx_e);

    for (const auto &e : edges)
        data.g->add_edge(e.node1, e.node2, e.weight);

}

GraphPtr loadFromFile(const std::string& path, unsigned int num_threads = std::thread::hardware_concurrency()) {

    //std::cout << "Loading graph from file: " << path << std::endl;

    //unsigned int num_threads = std::thread::hardware_concurrency();

    //auto start_time = std::chrono::high_resolution_clock::now();
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) throw std::invalid_argument("Impossible to open file");

    struct stat fileStat{};
    if (fstat(fd, &fileStat) == -1) throw std::invalid_argument("Impossible to get file size");


    auto intData = static_cast<unsigned int*>(mmap(nullptr, fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (intData == MAP_FAILED) throw std::invalid_argument("Impossible to map file");

    //la prima cella contiene il numero di nodi
    auto num_nodes = intData[0];

    //le due celle successive vengono castate in unsigned long che rappresenta il numero di archi
    auto num_edges = static_cast<unsigned long>(intData[2]) << sizeof(unsigned int)*8 | intData[1];

    auto g = std::make_shared<Graph>();
    g->nodes.resize(num_nodes);

    std::vector<std::thread> readers;
    std::barrier bar(num_threads);
    std::mutex mtx_e;

    thread_data td = {g, intData, num_nodes, num_edges, num_threads};

    for(int start = 0; start<num_threads; start++)
        readers.emplace_back(thread_reader, std::ref(td), start, std::ref(bar), std::ref(mtx_e));

    for(auto &t: readers)
        t.join();

    munmap(intData, fileStat.st_size);
    close(fd);

    //auto end_time = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    //std::cout << "Graph loaded in " << duration << " ms" << std::endl;

    return g;
}