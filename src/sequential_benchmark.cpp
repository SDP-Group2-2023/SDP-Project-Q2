#include "Graph.h"
#include "partitioning_algorithms.h"
#include <chrono>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

void graphPartitioning(Graph &graph, int num_partitions);
void coarseGraph(Graph &graph, vector<int> &partitions, int requested_num_partitions, int actual_num_partitions);
int calculate_weight(Graph &graph, Node n1, Node n2);

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: sequential_benchmark <path_to_graph>" << endl;
        return 1;
    }

    auto start_time = chrono::high_resolution_clock::now();
    Graph graph(argv[1]);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Duration: " << duration.count() << " ms" << endl;

    // graph.print();

    int num_partitions;
    cout << "Enter number of partitions: ";
    cin >> num_partitions;

    graphPartitioning(graph, num_partitions);

    return 0;
}

int calculatePartitionCost(Graph &graph, int partition, vector<int> &partitions) {
    int cost = 0;
    for (int i = 0; i < graph.get_num_nodes(); i++) {
        if (partitions[i] == partition) {
            cost += graph.get_node(i).weight;
        }
    }
    return cost;
}

void graphPartitioning(Graph &graph, int num_partitions) {
    graph.print();

    int num_nodes = graph.get_num_nodes();
    vector<bool> matched(num_nodes, false);
    set<Edge> matched_edges;
    Graph coarse_graph;
    int partition_index = 0;

    while (true) {
        try {
            Edge e             = graph.get_next_max_edge(matched);
            int source_id      = e.source;
            int dest_id        = e.dest;
            int distance       = e.weight;
            matched[source_id] = true;
            matched[dest_id]   = true;
            // cout << "Matched: " << source_id << " " <<dest_id << " " << distance << endl;
            matched_edges.insert(e);

            int source_weight = graph.get_node(source_id).weight;
            int dest_weight   = graph.get_node(dest_id).weight;

            graph.set_node_partition(source_id, partition_index);
            graph.set_node_partition(dest_id, partition_index);

            coarse_graph.add_node(partition_index, source_weight + dest_weight, -1);
        } catch (exception &e) {
            cout << e.what() << endl;
            break;
        }
        partition_index++;
    }

    auto edges = graph.get_edges();

    set<Edge>::iterator it;
    for (it = edges.begin(); it != edges.end(); ++it) {
        Edge tempEdge = *it;
        if (matched_edges.find(tempEdge) == matched_edges.end()) {
            Node source = graph.get_node(tempEdge.source);
            Node dest   = graph.get_node(tempEdge.dest);
            try {
                auto e = coarse_graph.get_edge(source.partition, dest.partition);
                coarse_graph.increase_edge(source.partition, dest.partition, tempEdge.weight);

            } catch (exception &e) {
                coarse_graph.add_edge(source.partition, dest.partition, tempEdge.weight);
            }
        }
    }

    coarse_graph.print();
}

int calculate_weight(Graph &graph, Node n1, Node n2) {
    cout << "Calculating weight between " << n1.id << " and " << n2.id << ": ";

    if (n1.id == -1 || n2.id == -1) {
        return 0;
    }
    try {
        int weight = graph.get_edge(n1.id, n2.id).weight;
        cout << weight << endl;
        return weight;
    } catch (exception &e) {
        cout << e.what() << endl;
        return 0;
    }
}