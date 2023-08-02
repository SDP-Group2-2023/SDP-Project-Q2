#include <iostream>
#include <list>
#include "partitioning.h"

using namespace std;
int main(int argc, char **argv){
    if(argc < 2){
        cout << "Usage: " << argv[0] << " <input file>"  << endl;
        return 1;
    }

    cout << "Loading graph..." << endl;
    auto start_time = chrono::high_resolution_clock::now();
    Graph*g = loadFromFile(argv[1]);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Time to load graph: " << duration.count() << "ms" << endl;

    int actual_num_partitions = g->V;

    list<Graph*> allGraphs;
    allGraphs.push_back(g);

    int iterations = 0;
    while( actual_num_partitions > 50 && iterations++ < 50){
        cout << "Iteration " << iterations << endl;
        Graph* coarsedGraph = coarseGraph(allGraphs.back(), 0);
        //coarsedGraph->print();
        actual_num_partitions = coarsedGraph->V;
        allGraphs.push_back(coarsedGraph);
    }

    allGraphs.back()->print();

    for(auto&gr : allGraphs)
        gr->free();

    cout << "Partitions: " << actual_num_partitions << endl;



}
