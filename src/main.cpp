#include <iostream>
#include <list>
#include "partitioning.h"
#include "timing/timing.h"

using namespace std;
int main(int argc, char **argv){
    if(argc < 2){
        cout << "Usage: " << argv[0] << " <input file>"  << endl;
        return 1;
    }

    auto g = loadFromFile(argv[1], 4);
    int requestedPartitions = 100;
    timing total_time;
    // partitioning_s(g, requestedPartitions);
    partitioning_p(g, requestedPartitions, 4);

    total_time.stop();
    auto elapsed = total_time.getDuration();
    cout << "Time elapsed: " << elapsed.count() << " ms" << endl;

    delete g;
}
