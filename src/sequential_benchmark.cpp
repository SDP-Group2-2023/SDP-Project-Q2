#include "Graph.h"
#include "partitioning_algorithms.h"
#include <chrono>
#include <iostream>
#include <set>
#include <vector>

using namespace std;
bool is_matched(set<Edge>& matched, Edge e_to_find, set<Edge>::iterator&it);
void graphPartitioning(Graph &graph, int num_partitions, int p_iteration);
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

    cout << "Read edges: " << graph.get_num_edges() << endl;

    // graph.print();

    int num_partitions = 0;
    //cout << "Enter number of partitions: ";
    //cin >> num_partitions;

    graphPartitioning(graph, num_partitions, 1);

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

void graphPartitioning(Graph &graph, int num_partitions, int p_iteration) {
    //graph.print();

    cout << "Start coarse graph: "<< p_iteration << endl;
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
            matched[source_id] = true;
            matched[dest_id]   = true;
            //cout << "Matched: " << source_id << " " <<dest_id << " " << distance << endl;
            matched_edges.insert(e);

            if(source_id > num_nodes-1 || dest_id > num_nodes-1){
                cout << "Source: " << source_id << endl;
                cout << "Dest: " << dest_id << endl;
            }

            int source_weight = graph.get_node(source_id).weight;
            int dest_weight   = graph.get_node(dest_id).weight;

            graph.set_node_partition(source_id, partition_index);
            graph.set_node_partition(dest_id, partition_index);

            coarse_graph.add_node(partition_index, source_weight + dest_weight, partition_index);
        } catch (exception &e) {
            cout << e.what() << endl;
            break;
        }
        partition_index++;
    }

    cout << "Partition index: " << partition_index << endl;

    //start_time = chrono::high_resolution_clock::now();
    auto edges = graph.get_edges();

    for(auto e: matched_edges)
        cout <<"Matched " << e.source << " " << e.dest << " " << e.weight << endl;

    int iteration = 0;

    set<Edge>::iterator it;
    auto match_it= matched_edges.begin();
    for (it = edges.begin(); it != edges.end(); ++it) {
        Edge tempEdge = *it;
        if (matched_edges.find(tempEdge) == matched_edges.end()) {

            //cout << "Edge not matched: "<< tempEdge.source << " " << tempEdge.dest << " " << tempEdge.weight << endl;
            Node source = graph.get_node(tempEdge.source);
            Node dest   = graph.get_node(tempEdge.dest);

            int source_partition = source.partition;
            int dest_partition   = dest.partition;
            if(source_partition < dest_partition) swap(source_partition, dest_partition);

            cout << "Source partition: " << source_partition << " Dest partition: " << dest_partition << endl;

            try {
                auto e = coarse_graph.get_edge(source.partition, dest.partition);
                coarse_graph.increase_edge(source.partition, dest.partition, tempEdge.weight);

            } catch (exception &e) {
                coarse_graph.add_edge(source.partition, dest.partition, tempEdge.weight);
            }
        }
    }

   /* end_time = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Duration: " << duration.count() << " ms" << endl;*/

    coarse_graph.print();

    if(num_partitions >= partition_index){
        cout << "Number of partitions is greater than number of nodes" << endl;
        return;
    }

    coarse_graph.resetMaxIterator();

    graphPartitioning(coarse_graph, num_partitions, p_iteration+1);

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