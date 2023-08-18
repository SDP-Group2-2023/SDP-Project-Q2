#include <iostream>
#include <list>
#include <thread>
#include "partitioning.h"

using namespace std;
int main(int argc, char **argv){
    if(argc < 2){
        cout << "Usage: " << argv[0] << " <input file>"  << endl;
        return 1;
    }

    auto g = loadFromFile_mmap(argv[1], 8);
    //auto g = loadFromFile_s(argv[1]);
    //auto g = loadFromFile_p(argv[1]);
    int requestedPartitions = 100;
    auto start = std::chrono::high_resolution_clock::now();
    parallel_partitioning(g, requestedPartitions, thread::hardware_concurrency());
    //sequential_partitioning(g, requestedPartitions);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    cout << "Time elapsed: " << elapsed.count() << " ms" << endl;

    delete g;
}
