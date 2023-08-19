#include "Graph.h"
#include <thread>
#include <barrier>

int colored = 0;
int last_color = 0;
int iterations = 0;
mutex color_mtx;
void colourGraphThread(Graph*g, vector<int>&randVal, int start, int num_threads, barrier<> &b, vector<int>&colours){

    int i = start;
    while(i<g->V()) {
        randVal[i] = rand();
        i+=num_threads;
    }

    b.arrive_and_wait();


    while(true){
        i = start;
        vector<int> buffer;

        color_mtx.lock();
        if (colored >= g->V()) {
            color_mtx.unlock();
            break;
        }
        color_mtx.unlock();

        while(i<g->V()) {
            if(colours[i] != -1){
                i+=num_threads;
                continue;
            }
            bool isMin = true;

            auto this_node = g->nodes[i];
            for (auto &other_node: this_node->get_neighbors()) {
                if (randVal[other_node->id] < randVal[this_node->id]) {
                    isMin = false;
                    break;
                }
                if(randVal[other_node->id] == randVal[this_node->id] && other_node->id < this_node->id){
                    isMin = false;
                    break;
                }
            }

            if (isMin) {
                buffer.emplace_back(i);
            }

            i+=num_threads;
        }

        b.arrive_and_wait();

        color_mtx.lock();
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
        color_mtx.unlock();
        b.arrive_and_wait();
    }


}

int colourGraph(Graph*g, vector<int>&colours, int num_threads){

    colored = 0;
    last_color = 0;
    iterations = 0;

    barrier b(num_threads);
    vector<int> randVal(g->V(), -1);
    vector<thread> threads(num_threads);
    for(int i = 0; i<num_threads; i++){
        threads[i] = thread(colourGraphThread, g, ref(randVal), i, num_threads, ref(b), ref(colours));
    }

    for(auto&t : threads)
        t.join();

    return last_color;

}

int get_max_edge(vector<shared_ptr<Edge>> edges, vector<bool>& matched_nodes){
    int max_edge_index = -1;
    int max_edge_weight = 0;

    for(int i = 0; i<edges.size(); i++){
        if(matched_nodes[edges[i]->node1->id] || matched_nodes[edges[i]->node2->id]) continue;
        if(edges[i]->weight > max_edge_weight){
            max_edge_weight = edges[i]->weight;
            max_edge_index = i;
        }
    }

    return max_edge_index;
}

void coarse_step(Graph*g, int colors_num, vector<bool>&matched_nodes,
                 Graph*coarse_graph, int&n_index, vector<int>&colours, int num_threads){

    for(int i = 0; i<colors_num; i++){
        for(auto&n: g->nodes){
            if(matched_nodes[n->id]) continue;

            if(colours[n->id] == i){
                int index = get_max_edge(n->edges, matched_nodes);
                if(index!=-1){
                    auto edge = n->edges[index];
                    Node*n1 = edge->node1;
                    Node*n2 = edge->node2;
                    matched_nodes[n1->id] = true;
                    matched_nodes[n2->id] = true;
                    Node*newNode = coarse_graph->add_node(n_index++, n1->weight + n2->weight);
                    n1->child = n2->child = newNode;
                }
            }
        }
    }
}


Graph*coarseGraph_p(Graph *g, int num_threads){

    vector<int> colours(g->V(), -1);
    auto colors_num = colourGraph(g, colours, num_threads);
    //cout << colors_num << endl;
    //for(int i = 0; i<g->V(); i++)cout << "Node " << i << " in color " << colours[i] << endl;

    cout << "coloured graph with " << colors_num << " colors" << endl;

    auto coarse_graph = new Graph();

    vector<bool> matched_nodes(g->V(), false);

    int n_index = 0;

    for(int i = 0; i<colors_num; i++){
        for(auto&n: g->nodes){
            if(matched_nodes[n->id]) continue;

            if(colours[n->id] == i){
                int index = get_max_edge(n->edges, matched_nodes);
                if(index!=-1){
                    auto edge = n->edges[index];
                    Node*n1 = edge->node1;
                    Node*n2 = edge->node2;
                    matched_nodes[n1->id] = true;
                    matched_nodes[n2->id] = true;
                    Node*newNode = coarse_graph->add_node(n_index++, n1->weight + n2->weight);
                    n1->child = n2->child = newNode;
                }
            }
        }
    }

    cout << "Coarse graph has " << coarse_graph->V() << " nodes" << endl;

    for(int i = 0; i<matched_nodes.size(); i++){
        //cout << i << " " << (matched_nodes[i] == 0?"false":"true") << endl;
        if(!matched_nodes[i]){
            auto n = g->nodes[i];
            Node*newNode = coarse_graph->add_node(n_index++, n->weight);
            n->child = newNode;
        }
    }

    for(auto& e : g->edges){
        if(e->node1->child != e->node2->child)
            coarse_graph->add_or_sum_edge(e->node1->child, e->node2->child, e->weight);
    }


    return coarse_graph;

}