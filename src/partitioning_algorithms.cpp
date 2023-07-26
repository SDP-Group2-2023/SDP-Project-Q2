#include "partitioning_algorithms.h"
#include <iostream>

using namespace std;
int sequential_partitioning(const Graph& graph, int num_partitions){
    if(num_partitions < 2 || num_partitions > graph.getNumNodes()){
        cout << "Invalid number of partitions" << endl;
        return -1;
    }

    std::vector<int> partition(graph.getNumNodes(), 0);
    //kernighanLin(graph, partition, num_partitions);
    return 0;
}

int parallel_partitioning(const Graph& graph, int num_partitions, int num_threads){
    return 0;
}
