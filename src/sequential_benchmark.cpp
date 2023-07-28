#include "partitioning_algorithms.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <set>

using namespace std;

void graphPartitioning(Graph& graph, int num_partitions);
void coarseGraph(Graph& graph, vector<int>& partitions, int requested_num_partitions, int actual_num_partitions);
int heavyEdgeCoarsening(Graph& graph, vector<int>& partitions);
void localImprovement(Graph& graph);
void uncoarseGraph(Graph& graph);
pair<int, int> find_max_edge(Graph& graph, vector<int>& partitions, vector<bool>& matched);

int main() {

    ifstream file("test.txt", std::ios::in);
    if (!file.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }
    int num_nodes, num_edges;
    file >> num_nodes >> num_edges;
    Graph graph(num_nodes);
    int id;
    int weight;
    for (int i = 0; i < num_nodes; i++) {
        file >> id >> weight;
        graph.addNodeWeight(id, weight);
    }
    int source;
    int dest;
    int distance;
    for (int i = 0; i < num_edges; i++) {
        file >> source >> dest >> distance;
        graph.addEdge(source, dest, distance);
    }
    file.close();

    //graph.print();
    cout << "Sequential partitioning" << endl;

    auto start_time = chrono::high_resolution_clock::now();
    graphPartitioning(graph, 2);
    auto end_time = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Duration: " << duration.count() << " ms" << endl;

    return 0;
}

int calculatePartitionCost(Graph& graph, int partition, vector<int>& partitions){
    int cost = 0;
    for(int i = 0; i<graph.getNumNodes(); i++){
        if(partitions[i] == partition){
            cost += graph.getNodeWeight(i);
        }
    }
    return cost;
}

void graphPartitioning(Graph& graph, int num_partitions){

    int num_nodes = graph.getNumNodes();
    vector<int> partitions(num_nodes);
    for(int i = 0; i<num_nodes; i++){
        partitions[i] = i;
    }
    coarseGraph(graph, partitions, num_partitions, num_nodes);
    localImprovement(graph);
    uncoarseGraph(graph);

    set<int> unique_partitions;
    for(int i = 0; i<num_nodes; i++){
        if(unique_partitions.find(partitions[i]) == unique_partitions.end()) {
            unique_partitions.insert(partitions[i]);
            cout << "Partition: " << partitions[i] <<" cost: "
                 << calculatePartitionCost(graph, partitions[i], partitions) << endl;
        }
    }
}

void coarseGraph(Graph& graph, vector<int>& partitions, int requested_num_partitions, int actual_num_partitions){
    if(actual_num_partitions <= requested_num_partitions){
        return;
    }

    actual_num_partitions = heavyEdgeCoarsening(graph, partitions);
    coarseGraph(graph, partitions, requested_num_partitions, actual_num_partitions);
}

int heavyEdgeCoarsening(Graph& graph, vector<int>& partitions){

    int num_nodes = graph.getNumNodes();
    vector<bool> matched(num_nodes, false);
    bool improvement = true;
    while(improvement) {
        improvement = false;
        pair<int, int> edge = find_max_edge(graph, partitions, matched);
        if(edge.first != -1 && edge.second != -1){
            improvement = true;
            matched[edge.first] = true;
            matched[edge.second] = true;
            partitions[edge.first] = partitions[edge.second];
        }
    }

    set<int> unique_partitions;
    for(int i = 0; i<num_nodes; i++){
        unique_partitions.insert(partitions[i]);
        cout << "Node:\t" << i << " Part:\t" << partitions[i] << endl;
    }
    cout << "Unique partitions: " << unique_partitions.size() << endl << endl;
    return (int)unique_partitions.size();
}

void localImprovement(Graph& graph){

}

void uncoarseGraph(Graph& graph){

}

pair<int, int> find_max_edge(Graph& graph, vector<int>& partitions, vector<bool>& matched){
    int num_nodes = graph.getNumNodes();
    int localMax = 0;
    int edgeWeight;
    int node1, node2;
    for(int i = 0; i<num_nodes; i++){
        for(int j = 0; j<num_nodes; j++){
            if(j!=i && !matched[i] && !matched[j] && partitions[i] != partitions[j]){
                edgeWeight = graph.getEdge(i, j);
                if(edgeWeight > localMax) {
                    localMax = edgeWeight;
                    node1 = i;
                    node2 = j;
                }
            }

        }
    }
    if(localMax == 0)
        return make_pair(-1, -1);
    //cout << "Max edge: " << node1 << " " << node2 << " " << localMax << endl;
    return make_pair(node1, node2);
}