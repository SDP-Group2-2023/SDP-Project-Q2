#include "Graph.h"


int colourGraph(Graph*g, vector<int>&colours){
    vector<int> randVal(g->V(), -1);

    for(int i = 0; i<g->V(); i++){
        randVal[i] = rand();
    }

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


Graph*coarseGraph_p(Graph *g){

    vector<int> colours(g->V(), -1);
    auto colors_num = colourGraph(g, colours);
    //cout << colors_num << endl;
    for(int i = 0; i<g->V(); i++)cout << "Node " << i << " in color " << colours[i] << endl;

    vector<shared_ptr<Edge>> matched_e;
    vector<bool> matched_nodes(g->V(), false);

    for(int i = 0; i<colors_num; i++){
        for(auto&n: g->nodes){
            if(matched_nodes[n->id]) continue;

            if(colours[n->id] == i){
                int index = get_max_edge(n->edges, matched_nodes);
                if(index!=-1){
                    matched_e.push_back(n->edges[index]);
                    matched_nodes[n->edges[index]->node1->id] = true;
                    matched_nodes[n->edges[index]->node2->id] = true;
                }
            }
        }
    }

    for(int i = 0; i<matched_nodes.size(); i++){
        cout << i << " " << (matched_nodes[i] == 0?"false":"true") << endl;
    }

    for(auto&e: matched_e){
       cout << e->node1->id << " " << e->node2->id << endl;
    }

    return g;

}