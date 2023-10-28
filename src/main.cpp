#include <iostream>
#include "partitioning.h"
#include "timing/timing.h"


int main(int argc, char **argv){
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " <input file>"  << std::endl;
        return 1;
    }

    auto g = loadFromFile(argv[1]);

    int requestedPartitions = 100;
    timing total_time;
    
    #ifndef PARALLEL_BUILD
    partitioning_s(g, requestedPartitions);
    #else
    partitioning_p(g, requestedPartitions, 4);
    #endif

    total_time.stop();
    auto elapsed = total_time.getDuration();
    std::cout << "Time elapsed: " << elapsed.count() << " ms" << std::endl;

}
