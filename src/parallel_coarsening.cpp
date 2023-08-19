#include "Graph.h"
#include <thread>
#include <barrier>

int colored = 0;
int last_color = 0;
int iterations = 0;

/**
 * Compares two nodes based on their random value.
 * If the random value is the same, the node with the smaller id is chosen.
 * @param n1
 * @param n2
 * @param randVal
 * @return true if n1 is bigger than n2, false otherwise
 */
bool compare_nodes(Node*n1, Node*n2, vector<int>&randVal){
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
void colourGraphThread(Graph*g, vector<int>&randVal, int start,int num_threads,
                       barrier<> &b, mutex&color_mtx, vector<int>&colours){

    unique_lock<mutex> thread_lock{color_mtx, defer_lock};

    for(int i = start; i<g->V(); i+=num_threads){
        randVal[i] = rand();
    }

    b.arrive_and_wait();

    while(true){
        vector<int> buffer;

        thread_lock.lock();
        if (colored >= g->V())
            break;
        thread_lock.unlock();

        for(int i = start; i<g->V(); i+=num_threads){
            if(colours[i] != -1)
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
        if(!buffer.empty()) {
            for (auto minNode: buffer) {
                randVal[minNode] = INT_MAX;
                colours[minNode] = last_color;
                colored++;
            }
        }
        iterations++;
        if(iterations == num_threads){
            iterations = 0;
            last_color++;
        }
        thread_lock.unlock();
        b.arrive_and_wait();
    }


}

/**
 * Colours the graph using the Luby's algorithm.
 * @param g the graph to color
 * @param colours the vector to store the colours in
 * @param num_threads the number of threads to use
 * @return the number of colours used
 */
int colourGraph(Graph*g, vector<int>&colours, int num_threads){

    colored = 0;
    last_color = 0;
    iterations = 0;

    mutex color_mtx;
    barrier b(num_threads);
    vector<int> randVal(g->V(), -1);
    vector<thread> threads(num_threads);
    for(int i = 0; i<num_threads; i++)
        threads[i] = thread(colourGraphThread, g, ref(randVal), i, num_threads,
                            ref(b), ref(color_mtx) , ref(colours));

    for(auto&t : threads)
        t.join();

    return last_color;
}

shared_ptr<Edge> get_max_edge(const vector<shared_ptr<Edge>>& edges, vector<bool>& matched_nodes){
    int max_edge_weight = INT_MIN;

    shared_ptr<Edge> max_edge = nullptr;
    for(auto&e: edges){
        if(matched_nodes[e->node1->id] || matched_nodes[e->node2->id]) continue;
        if(e->weight > max_edge_weight){
            max_edge_weight = e->weight;
            max_edge = e;
        }
    }

    if(max_edge == nullptr)
        throw runtime_error("No max edge found");

    return max_edge;
}

Graph*coarseGraph_p(Graph *g, int num_threads){

    vector<int> colours(g->V(), -1);

    auto colors_num = colourGraph(g, colours, num_threads);
    auto coarse_graph = new Graph();

    vector<bool> matched_nodes(g->V(), false);
    int n_index = 0;
    for(int i = 0; i<colors_num; i++){
        for(auto&n: g->nodes){
            if(matched_nodes[n->id]) continue;
            if(colours[n->id] == i){
                try{
                    auto edge = get_max_edge(n->edges, matched_nodes);
                    Node*n1 = edge->node1;
                    Node*n2 = edge->node2;
                    matched_nodes[n1->id] = matched_nodes[n2->id] = true;
                    Node*newNode = coarse_graph->add_node(n_index++, n1->weight + n2->weight);
                    n1->child = n2->child = newNode;
                }catch (exception&e){
                    n->child = coarse_graph->add_node(n_index++, n->weight);
                }
            }
        }
    }


    for(auto& e : g->edges){
        if(e->node1->child != e->node2->child)
            coarse_graph->add_or_sum_edge(e->node1->child, e->node2->child, e->weight);
    }

    return coarse_graph;
}