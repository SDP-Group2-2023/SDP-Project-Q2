#include <iostream>
#include <list>
#include "partitioning.h"

using namespace std;
int main(int argc, char **argv){
    if(argc < 2){
        cout << "Usage: " << argv[0] << " <input file>"  << endl;
        return 1;
    }

    auto g = loadFromFile(argv[1]);
    int requestedPartitions = 100;
    partitioning(g, requestedPartitions);

    delete g;
}
