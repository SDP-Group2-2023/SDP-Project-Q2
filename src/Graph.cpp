#include "Graph.h"

using namespace std;

/* bool Graph::cmp(const Edge &a, const Edge &b) {
    return a.weight > b.weight;
} */

void Graph::add_node(int id, int weight){
	Node nNode{id, weight};

	unique_lock<mutex> lock(mNode);	
	nodes.emplace_back(nNode);
}

void Graph::add_edge(int source, int dest, int weight){
	if(source < dest) swap(source, dest);
	Edge nEdge{source, dest, weight};

	unique_lock<mutex> lock(mEdge);	
	edges.insert(nEdge);
}


Node Graph::get_node(int id){
    return nodes[id];
}

Edge Graph::get_edge(int source, int dest){
    if(source < dest)
        swap(source, dest);

    set<Edge>::iterator it;
    for(it = edges.begin(); it!= edges.end(); ++it){
        Edge tempEdge = *it;
        if(tempEdge.source == source && tempEdge.dest == dest)
            return tempEdge;
    }

    throw runtime_error("Edge not found");
}

Edge Graph::get_next_max_edge(vector<bool>& matched, vector<int>& partitions){
    set<Edge>::iterator it;
    int source;
    int dest;
    for (it = edges.begin(); it != edges.end(); ++it)
    {
        source = it->source;
        dest = it->dest;
        if (!matched[source] && !matched[dest] && partitions[source] != partitions[dest])
            return (*it);
    }

    throw runtime_error("No more edges");
}


int Graph::get_num_edges(){
    return edges.size();
}

int Graph::get_num_nodes(){
    return nodes.size();
}


