#include <iostream>
#include <list>
#include "partitioning.h"

using namespace std;
int main(int argc, char **argv){
    if(argc < 2){
        cout << "Usage: " << argv[0] << " <input file>"  << endl;
        return 1;
    }

    Graph*g = loadFromFile(argv[1]);

    int requestedPartitions = 50;
    partitioning(g, requestedPartitions);

    delete g;
}
