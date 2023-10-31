#include <iostream>
#include "../partitioning.h"
#include "../timing/timing.h"

int main(int argc, char**argv){
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " <input file>"  << std::endl;
        return 1;
    }

    auto g = loadFromFile(argv[1]);

    const int K = 10;

    for(int j = 1; j <= 8; j++) {

        long long timings = 0;
        long long min = LONG_LONG_MAX;
        long long max = LONG_LONG_MIN;

        for (int i = 0; i < K; i++) {

            //(i * 100) / K % 5 == 0 ? std::cout << (i * 100) / K << "%" << std::endl : std::cout;

            auto start_time = std::chrono::high_resolution_clock::now();

            partitioning_p(g, 100, j);

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            timings += duration;
            if (duration > max) max = duration;
            if (duration < min) min = duration;
        }

        std::cout << "THREADS: " << j << std::endl;
        std::cout << "MIN: " << min << std::endl;
        std::cout << "MAX: " << max << std::endl;
        std::cout << "AVG: " << timings / K << std::endl;
    }
}