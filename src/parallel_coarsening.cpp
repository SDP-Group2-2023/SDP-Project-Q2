#include "Graph.h"
#include <thread>

void colourGraphThread(Graph*g, vector<int>&randVal, int start, int num_threads){

    int i = start;
    while(i<g->V()) {
        randVal[i] = rand();
        i+=num_threads;
    }

}

int colourGraph(Graph*g, vector<int>&colours, int num_threads){

    vector<int> randVal(g->V(), -1);
    vector<thread> threads(num_threads);
    for(int i = 0; i<num_threads; i++){
        threads[i] = thread(colourGraphThread, g, ref(randVal), i, num_threads);
    }

    for(auto&t : threads)
        t.join();

    int colorIndex = 0;
    int colored = 0;

    while(colored < g->V()) {
        vector<bool> matched(g->V(), false);
        for(int i = 0; i<g->V(); i++) {
            if(colours[i] != -1 || matched[i]) continue;

            bool isMin = true;

            for(auto&n: g->nodes[i]->get_neighbors()){
                if(colours[n->id] == -1 && randVal[i] > randVal[n->id]){
                    isMin = false;
                    break;
                }
            }

            if(isMin){
                colours[i] = colorIndex;
                colored++;
                matched[i] = true;
                for(auto&n: g->nodes[i]->get_neighbors()){
                    matched[n->id] = true;
                }
            }
        }
        colorIndex++;
    }

    return colorIndex;

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
    for(int i = 0; i<g->V(); i++)cout << "Node " << i << " in color " << colours[i] << endl;

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

    /*vector<thread> threads(num_threads);

    for(int i = 0; i< num_threads; i++)
        threads[i] = thread();*/

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