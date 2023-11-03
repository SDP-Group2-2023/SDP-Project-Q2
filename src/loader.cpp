#include "Graph.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <thread>
#include <barrier>
#include <mutex>

//struttura dati per salvare temporaneamente le informazioni sugli edge letti
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
};

const unsigned int starting_offset = 3; //the size of num_nodes + num_edges in the first line of the file
unsigned int reading_step; //viene assegnato dopo
unsigned int nodeCounter = 0;
unsigned int edgeCounter = 0;

/**
 * Funzione che descrive il comportamento di un thread di lettura
 * @param data una struttura dati che contiene il file da leggere e le sue caratteristiche
 * @param bar una barriera
 * @param mtx_n_counter un mutex per l'accesso esclusivo al nodeCounter
 * @param mtx_e_counter un mutex per l'accesso esclusivo a edgeCounter
 * @param mtx_e_adder un mutex per l'aggiunta di edge al grafo in maniera esclusiva
 */
void thread_reader(const thread_data& data, std::barrier<>& bar,
                   std::mutex& mtx_n_counter, std::mutex& mtx_e_counter, std::mutex& mtx_e_adder){

    unsigned int node_now = 0;
    unsigned int cursor;

    while (node_now < data.num_nodes){
        mtx_n_counter.lock();
        node_now = nodeCounter;
        nodeCounter += reading_step;
        mtx_n_counter.unlock();
        cursor = starting_offset + node_now * 2;

        for(int i = 0; i < reading_step; i++){
            if (node_now + i >= data.num_nodes) break;
            unsigned int node_id = data.file_data[cursor];
            unsigned int node_weight = data.file_data[cursor + 1];
            data.g->add_node_with_index(node_id, node_weight);
            cursor += 2;

        }
    }

    bar.arrive_and_wait();

    unsigned int edge_now = 0;

    //questo buffer permette di leggere e accumulare un certo numero di edge e aggiungerli al grafo
    //senza dover acquisire il mutex troppe volte, diminuendo così parte dell'overhead
    std::vector<m_edge> e_buffer;
    while(edge_now < data.num_edges){
        mtx_e_counter.lock();
        edge_now = edgeCounter;
        edgeCounter += reading_step;
        mtx_e_counter.unlock();
        cursor = starting_offset + data.num_nodes * 2 + edge_now * 3;

        for(int i = 0; i < reading_step; i++){
            if (edge_now + i >= data.num_edges) break;
            auto node1 = data.file_data[cursor];
            auto node2 = data.file_data[cursor + 1];
            auto weight = data.file_data[cursor + 2];
            e_buffer.emplace_back(node1, node2, weight);
            cursor += 3;
        }

        //verificato sperimentalmente che questa mossa migliora notevolmente le prestazioni
        if(mtx_e_adder.try_lock()){
            for(auto& e : e_buffer)
                data.g->add_edge(e.node1, e.node2, e.weight);
            e_buffer.clear();
            mtx_e_adder.unlock();
        }
    }

    if(e_buffer.empty()) return;

    mtx_e_adder.lock();
    for(auto& e : e_buffer)
        data.g->add_edge(e.node1, e.node2, e.weight);
    mtx_e_adder.unlock();

}

//sperimentalmente dimostrato che se ogni thread legge il 10% dei nodi ad ogni iterazione è più veloce

/**
 *
 * @param path il path del file binario che descrive il grafo
 * @param num_threads il numero di thread con cui si vuole leggere il grafo
 * @param myStep la percentuale di nodi che ogni thread leggerà ad ogni iterazione
 * @return uno shared_ptr del grafo appena letto
 */
GraphPtr loadFromFile(const std::string& path,
                      unsigned int num_threads = std::thread::hardware_concurrency(),
                      unsigned int myStep = 10 ) {

    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) throw std::invalid_argument("Impossible to open file");

    struct stat fileStat{};
    if (fstat(fd, &fileStat) == -1) throw std::invalid_argument("Impossible to get file size");

    //con questa operazione il file viene letto una volta sola ma è possibile accedervi in maniera parallela
    auto intData = static_cast<unsigned int*>(mmap(nullptr, fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (intData == MAP_FAILED) throw std::invalid_argument("Impossible to map file");

    //la prima cella contiene il numero di nodi
    auto num_nodes = intData[0];
    reading_step = num_nodes*myStep/100;


    //le due celle successive vengono castate in unsigned long che rappresenta il numero di archi
    auto num_edges = static_cast<unsigned long>(intData[2]) << sizeof(unsigned int)*8 | intData[1];

    auto g = std::make_shared<Graph>();
    g->nodes.resize(num_nodes);

    //inizializzazione delle strutture dati per la lettura parallela
    std::vector<std::thread> readers;
    std::barrier bar(num_threads);
    std::mutex mtx_n_counter, mtx_e_counter, mtx_e_adder;

    thread_data td = {g, intData, num_nodes, num_edges};

    for(int i = 0; i<num_threads; i++)
        readers.emplace_back(thread_reader, std::ref(td), std::ref(bar),
                             std::ref(mtx_n_counter), std::ref(mtx_n_counter), std::ref(mtx_e_counter));

    for(auto &t: readers)
        t.join();

    nodeCounter = 0;
    edgeCounter = 0;

    munmap(intData, fileStat.st_size);
    close(fd);

    return g;
}