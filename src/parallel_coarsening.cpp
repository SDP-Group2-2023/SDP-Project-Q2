#include "Graph.h"
#include <thread>
#include <barrier>
#include <mutex>
#include <vector>

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
                       barrier<> &b, mutex&color_mtx,
                       int&colored, int&last_color, int&iterations){

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
        if(!buffer.empty()) {
            for (auto minNode: buffer) {
                randVal[minNode] = INT_MAX;
                g->colours[minNode] = last_color;
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
int colourGraph(Graph*g, int num_threads){

    int colored = 0;
    int last_color = 0;
    int iterations = 0;

    mutex color_mtx;
    barrier b(num_threads);
    vector<int> randVal(g->V(), -1);
    vector<thread> threads(num_threads);
    for(int i = 0; i<num_threads; i++)
        threads[i] = thread(colourGraphThread, g, ref(randVal), i, num_threads,
                            ref(b), ref(color_mtx) ,
                            ref(colored), ref(last_color), ref(iterations));

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

void coarse_step(Graph*original_graph, Graph*coarse_graph, int start, int num_threads, mutex&mtx, barrier<>&b,
                 int max_colour, vector<bool>&matched_nodes, vector<int>&matched_index, int&n_index){
    int colour = 0;

    while (colour < max_colour){
        for(int i = start; i<original_graph->V(); i+=num_threads){

            mtx.lock();
            if(matched_nodes[i]) {
                mtx.unlock();
                continue;
            }
            mtx.unlock();

            if(original_graph->colours[i] == colour){
                Node*n = original_graph->nodes[i];
                try {
                    auto e = get_max_edge(n->edges, matched_nodes);
                    matched_index[e->node1->id] = e->node2->id;
                    matched_index[e->node2->id] = e->node1->id;
                }
                catch (runtime_error& e){
                    mtx.lock();
                    matched_nodes[i] = true;
                    n->child = coarse_graph->add_node(n_index++, n->weight);
                    mtx.unlock();
                }
            }
        }

        b.arrive_and_wait();

        for(int i = start; i<original_graph->V(); i+=num_threads){
            mtx.lock();
            if(matched_nodes[i]) {
                mtx.unlock();
                continue;
            }
            mtx.unlock();

            if(original_graph->colours[i] == colour){

                Node*n1 = original_graph->nodes[i];
                Node*n2 = original_graph->nodes[matched_index[i]];

                if(matched_index[n2->id] == n1->id){
                    mtx.lock();
                    matched_nodes[n1->id] = matched_nodes[n2->id] = true;
                    n1->child = n2->child = coarse_graph->add_node(n_index++, n1->weight + n2->weight );
                    mtx.unlock();
                }
            }
        }
        colour++;
    }

    b.arrive_and_wait();

    vector<Node*> buffer;

    for(int i = start; i<original_graph->V(); i+=num_threads){
        if(matched_nodes[i]) continue;
        Node*n = original_graph->nodes[i];
        buffer.emplace_back(n);
    }

    for(auto& n: buffer){
        mtx.lock();
        n->child = coarse_graph->add_node(n_index++, n->weight);
        mtx.unlock();
    }


}

Graph*coarseGraph_p(Graph *g, int num_threads){

    g->colours = vector<int>(g->V(), -1);

    g->num_colours = colourGraph(g, num_threads);
    auto coarse_graph = new Graph();

    mutex mtx;
    barrier<> b(num_threads);

    vector<bool> matched_nodes(g->V(), false);
    vector<int> matched_index(g->V(), -1);
    int n_index = 0;
    vector<thread> threads(num_threads);

    for(int i = 0; i<num_threads; i++)
        threads[i] = thread(coarse_step, g, coarse_graph, i, num_threads, ref(mtx), ref(b), g->num_colours,
                            ref(matched_nodes), ref(matched_index), ref(n_index));

    for(auto&t: threads)
        t.join();

    for(auto& e : g->edges){
        if(e->node1->child != e->node2->child)
            coarse_graph->add_or_sum_edge(e->node1->child, e->node2->child, e->weight);
    }

    return coarse_graph;
}