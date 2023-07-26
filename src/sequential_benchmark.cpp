#include "partitioning_algorithms.h"
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;

int main() {

    ifstream file("test.txt", std::ios::in);
    if (!file.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }
    int num_nodes, num_edges;
    file >> num_nodes >> num_edges;
    Graph graph(num_nodes, num_edges);
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
    sequential_partitioning(graph, 3);
    auto end_time = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Duration: " << duration.count() << " ms" << endl;

    return 0;
}