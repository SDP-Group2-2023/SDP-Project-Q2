#include "Graph.h"
#include <thread>
#include <barrier>
#include <vector>
#include <mutex>

/**
 * Compares two nodes based on their random value.
 * If the random value is the same, the node with the smaller id is chosen.
 * @param n1
 * @param n2
 * @param randVal
 * @return true if n1 is bigger than n2, false otherwise
 */
bool compare_nodes(const NodePtr & n1, const NodePtr & n2, std::vector<unsigned int>&randVal){
    return (randVal[n2->id] < randVal[n1->id] || (randVal[n2->id] == randVal[n1->id] && n2->id < n1->id));
}

/**
 * Colours the graph using the Luby's algorithm.
 * @param g the graph to color
 * @param colours the vector to store the colours in
 * @param num_threads the number of threads to use
 * @param randVal the random values of the nodes
 * @param start the start index of the thread
 * @param b the barrier to use
 * @param color_mtx the mutex to use
 */
void colourGraphThread(GraphPtr &g, std::vector<unsigned int>&randVal, int start,int num_threads,
                       std::barrier<> &b, std::mutex&color_mtx,
                       int&colored, int&last_color, int&iterations){

    std::unique_lock<std::mutex> thread_lock{color_mtx, std::defer_lock};

    for(int i = start; i<g->V(); i+=num_threads){
        randVal[i] = (unsigned int)random();
    }

    while(true){
        b.arrive_and_wait();
        std::vector<int> buffer;

        thread_lock.lock();
        if (colored >= g->V())
            break;
        thread_lock.unlock();

        for(int i = start; i<g->V(); i+=num_threads){
            if(g->colours[i] != -1)
                continue;

            bool isMin = true;
            auto this_node = g->nodes[i];

            for (auto &other_node: this_node->get_neighbors()) {
                if (compare_nodes(this_node, other_node, randVal)) {
                    isMin = false;
                    break;
                }
            }

            if (isMin) buffer.emplace_back(i);

        }

        b.arrive_and_wait();

        thread_lock.lock();
        for (auto& minNode: buffer) {
            randVal[minNode] = UINT_MAX	;
            g->colours[minNode] = last_color;
            colored++;
        }
        iterations++;
        if(iterations == num_threads){
            iterations = 0;
            last_color++;
        }
        thread_lock.unlock();
    }
}

/**
 * Colours the graph using the Luby's algorithm.
 * @param g the graph to color
 * @param colours the vector to store the colours in
 * @param num_threads the number of threads to use
 * @return the number of colours used
 */
unsigned int colourGraph(GraphPtr&g, int num_threads){

     int colored = 0;
     int last_color = 0;
     int iterations = 0;

    std::mutex color_mtx;
    std::barrier b(num_threads);
    std::vector<unsigned int> randVal(g->V(), 0);
    std::vector<std::thread> threads(num_threads);

    for(int i = 0; i<num_threads; i++)
        threads[i] = std::thread(colourGraphThread, std::ref(g), ref(randVal), i, num_threads,
                                 std::ref(b), std::ref(color_mtx) ,
                                 std::ref(colored), std::ref(last_color), std::ref(iterations));

    for(auto&t : threads) t.join();
    return last_color;
}

EdgePtr old_get_max_edge(const EdgePtrArr& edges, std::vector<bool>& matched_nodes){
    unsigned int max_edge_weight = 0;

    EdgePtr max_edge = nullptr;
    for(const auto&e: edges){
        if(matched_nodes[e->node1.lock()->id] || matched_nodes[e->node2.lock()->id]) continue;
        if(e->weight > max_edge_weight){
            max_edge_weight = e->weight;
            max_edge = e;
        }
    }

    if(max_edge == nullptr)
        throw std::runtime_error("No max edge found");

    return max_edge;
}


bool get_max_edge(const EdgePtrArr& edges, std::vector<bool>& matched_nodes, EdgePtr& retVal){
    unsigned int max_edge_weight = 0;

    EdgePtr max_edge = nullptr;
    for(const auto&e: edges){
        if(matched_nodes[e->node1.lock()->id] || matched_nodes[e->node2.lock()->id]) continue;
        if(e->weight > max_edge_weight){
            max_edge_weight = e->weight;
            max_edge = e;
        }
    }

    if(max_edge == nullptr) {
        return false;
    }

    retVal = max_edge;

    return true;
}


void coarse_step(const GraphPtr& original_graph, const GraphPtr& coarse_graph, int start, int num_threads,
                 std::mutex&mtx, std::barrier<>&b,  int max_colour, std::vector<bool>&matched_nodes,
                 std::vector<unsigned int>&matched_index, int&n_index){
    int colour = 0;

    while (colour < max_colour){
        for(int i = start; i<original_graph->V(); i+=num_threads){

            std::unique_lock lock(mtx);
            if(matched_nodes[i]) continue;
            lock.unlock();

            if(original_graph->colours[i] == colour){
                const auto n = original_graph->nodes[i];
                EdgePtr e;
                bool result = get_max_edge(n->edges, matched_nodes,e);
                if(result){
                    matched_index[e->node1.lock()->id] = e->node2.lock()->id;
                    matched_index[e->node2.lock()->id] = e->node1.lock()->id;
                }else{
                    std::unique_lock lock2(mtx);
                    matched_nodes[i] = true;
                    n->child = coarse_graph->add_node(n_index++, n->weight);
                    lock2.unlock();
                }
            }
        }

        b.arrive_and_wait();

        for(int i = start; i<original_graph->V(); i+=num_threads){
            std::unique_lock lock3(mtx);
            if(matched_nodes[i]) continue;
            lock3.unlock();

            if(original_graph->colours[i] == colour){

                const auto n1 = original_graph->nodes[i];
                const auto n2 = original_graph->nodes[matched_index[i]];

                if(matched_index[n2->id] == n1->id){
                    std::unique_lock lock4(mtx);
                    matched_nodes[n1->id] = matched_nodes[n2->id] = true;
                    n1->child = n2->child = coarse_graph->add_node(n_index++, n1->weight + n2->weight );
                    lock4.unlock();
                }
            }
        }
        colour++;
    }

    b.arrive_and_wait();

    NodePtrArr buffer;
    for(int i = start; i<original_graph->V(); i+=num_threads){
        if(matched_nodes[i]) continue;
        const auto n = original_graph->nodes[i];
        buffer.emplace_back(n);
    }

    for(const auto& n: buffer){
        std::unique_lock lock5(mtx);
        n->child = coarse_graph->add_node(n_index++, n->weight);
        lock5.unlock();
    }


}

/**
 * Si calcola un coarsed graph in maniera parallela
 * @param g il grafo di partenza
 * @param num_threads il numero di thread da usare
 * @return il grafo calcolato
 */
GraphPtr coarseGraph_p(GraphPtr&g, int num_threads){

    g->colours = std::vector<int>(g->V(), -1);

    //prima si colora il grafo attraverso una versione modificata dell'algoritmo di Luby
    g->num_colours = colourGraph(g, num_threads);
    auto coarse_graph = std::make_shared<Graph>();

    std::mutex mtx;
    std::barrier b(num_threads);

    std::vector<bool> matched_nodes(g->V(), false);
    std::vector<unsigned int> matched_index(g->V(), 0);
    int n_index = 0;
    std::vector<std::thread> threads;

    //vengono instanziati un tot di thread per effettuare il partizionamento basato sul colore
    for(int i = 0; i<num_threads; i++)
        threads.emplace_back(coarse_step, std::ref(g), std::ref(coarse_graph), i,
                                 num_threads, std::ref(mtx), std::ref(b), g->num_colours,
                                 std::ref(matched_nodes), std::ref(matched_index), std::ref(n_index));

    for(auto&t: threads)
        t.join();

    //vengono calcolate le distanze tra i nodi nel nuovo grafo
    for(const auto& e : g->edges){
        if(e->node1.lock()->child->id != e->node2.lock()->child->id)
            coarse_graph->add_or_sum_edge(e->node1.lock()->child, e->node2.lock()->child, e->weight);
    }

    return coarse_graph;
}