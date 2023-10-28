#include "Graph.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <thread>
#include <barrier>

struct m_edge{
    unsigned int node1;
    unsigned int node2;
    unsigned int weight;
};
void thread_reader(std::shared_ptr<Graph>& g, const unsigned int*filedata, unsigned int num_nodes,
                   unsigned long num_edges, int start, int step, std::barrier<>& bar, std::mutex& mtx_e){

    unsigned int n_id;
    unsigned int n_weight;
    unsigned int cursor = start*2 + 3;
    while(cursor< num_nodes*2 + 3){
        n_id = filedata[cursor];
        n_weight = filedata[cursor+1];
        g->add_node_with_index(n_id, n_weight);
        cursor += step*2;
    }

    m_edge e_temp{};
    std::vector<m_edge> edges;
    cursor = start * 3 + 3 + num_nodes*2;
    while(cursor< num_nodes*2 + 3 + num_edges*3){
        e_temp.node1 = filedata[cursor];
        e_temp.node2 = filedata[cursor+1];
        e_temp.weight = filedata[cursor+2];
        edges.emplace_back(e_temp);
        cursor += step*3;
    }

    bar.arrive_and_wait();

    mtx_e.lock();
    for (auto &e : edges) {
        g->add_edge(e.node1, e.node2, e.weight);
    }
    mtx_e.unlock();
}

std::shared_ptr<Graph> loadFromFile(const std::string& path) {

    int num_threads = 4;
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

    std::shared_ptr<Graph> g(new Graph());
    g->nodes.resize(num_nodes);

    std::vector<std::thread> readers(num_threads);
    std::barrier bar(num_threads);
    std::mutex mtx_e;


    for(int i = 0; i<num_threads; i++){
        readers[i] = std::thread(thread_reader, ref(g), intData, num_nodes, num_edges, i, num_threads, ref(bar), ref(mtx_e));
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