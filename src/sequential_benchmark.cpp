#include "partitioning_algorithms.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <set>

using namespace std;

void graphPartitioning(Graph& graph, int num_partitions);
void coarseGraph(Graph& graph, vector<int>& partitions, int requested_num_partitions, int actual_num_partitions);
int heavyEdgeCoarsening(Graph& graph, vector<int>& partitions);
void localImprovement(Graph& graph);
void uncoarseGraph(Graph& graph);
pair<int, int> find_max_edge(Graph& graph, vector<int>& partitions, vector<bool>& matched);

int main(int argc, char** argv) {

    if(argc != 2){
        cout << "Usage: sequential_benchmark <path_to_graph>" << endl;
        return 1;
    }

    auto start_time = chrono::high_resolution_clock::now();
    Graph graph(argv[1]);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Duration: " << duration.count() << " ms" << endl;

    //graph.print();

    vector<bool> matched(graph.get_num_nodes(), false);
    vector<int> partitions(graph.get_num_nodes());
    for(int i = 0; i<graph.get_num_nodes(); i++){
        partitions[i] = i;
    }

    for(int i = 0; i<5; i++) {
        auto nextEdge = graph.get_next_max_edge(matched, partitions);

        cout << "Max edge: source " << nextEdge.source
             << ", dest: " << nextEdge.dest
             << ", weight: " << nextEdge.weight << endl;

        matched[nextEdge.source] = true;
        matched[nextEdge.dest] = true;
        partitions[nextEdge.dest] = partitions[nextEdge.source];


    }
    //graphPartitioning(graph, 50);


    return 0;
}

int calculatePartitionCost(Graph& graph, int partition, vector<int>& partitions){
    int cost = 0;
    for(int i = 0; i<graph.get_num_nodes(); i++){
        if(partitions[i] == partition){
            cost += graph.get_node(i).weight;
        }
    }
    return cost;
}

void graphPartitioning(Graph& graph, int num_partitions){

    int num_nodes = graph.get_num_nodes();
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
    cout << "actual: " << actual_num_partitions << endl;
    actual_num_partitions = heavyEdgeCoarsening(graph, partitions);
    coarseGraph(graph, partitions, requested_num_partitions, actual_num_partitions);
}

int heavyEdgeCoarsening(Graph& graph, vector<int>& partitions){

    int num_nodes = graph.get_num_nodes();
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
    int num_nodes = graph.get_num_nodes();
    int localMax = 0;
    int edgeWeight;
    int node1, node2;
    for(int i = 0; i<num_nodes; i++){
        for(int j = 0; j<num_nodes; j++){
            edgeWeight = graph.get_edge(i, j).weight;
            if(j!=i && !matched[i] && !matched[j] && partitions[i] != partitions[j] && edgeWeight != 0){
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