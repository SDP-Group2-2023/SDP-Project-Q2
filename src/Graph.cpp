#include "Graph.h"
#include <iostream>

using namespace std;

Graph::Graph() {}

void Graph::add_node(int id, int weight, int partition){
	Node nNode{id, weight, partition};

    lock_guard<mutex> lock(mNode);
    nodes.emplace_back( nNode);
}

void Graph::add_edge(int source, int dest, int weight){
	if(source < dest) swap(source, dest);
	Edge nEdge{source, dest, weight};

	lock_guard<mutex> lock(mEdge);
	edges.insert(nEdge);
    //nodes[source].edges.insert(nEdge);
    //nodes[dest].edges.insert(nEdge);
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

Edge Graph::get_next_max_edge(vector<bool>& matched){
    set<Edge>::iterator it;
    int source, dest;
    for (it = edges.begin(); it != edges.end(); ++it)
    {
        source = it->source;
        dest = it->dest;
        if (!matched[source] && !matched[dest])
            return (*it);
    }

    throw runtime_error("No more edges");
}


int Graph::get_num_edges(){
    return (int)edges.size();
}

int Graph::get_num_nodes(){
    return (int)nodes.size();
}

void Graph::print(){
    set<Edge>::iterator it;
    for(it = edges.begin(); it!= edges.end(); ++it){
        Edge tempEdge = *it;
        cout << tempEdge.source << " " << tempEdge.dest << " " << tempEdge.weight << endl;
    }
}

void Graph::set_node_partition(int id, int partition){
    nodes[id].partition = partition;
}

set<Edge> Graph::get_edges(){
    return edges;
}
void Graph::increase_edge(int source, int dest, int weight_increment){
    Edge e = this->get_edge(source, dest);
    this->edges.erase(e);
    e.weight += weight_increment;
    this->edges.insert(e);
}
