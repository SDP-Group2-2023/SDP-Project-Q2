#include <iostream>
#include <fstream>
#include <ctime>
#include "../partitioning.h"
#include "../timing/timing.h"

int main(int argc, char**argv){
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " <input file>"  << std::endl;
        return 1;
    }

    std::time_t result = std::time(nullptr);



    //std::string timestamp  =std::asctime(std::localtime(&result));
    //timestamp.pop_back();
    //for(auto& c : timestamp) if(c==' ') c='_';

    //std::cout << timestamp << std::endl;

    auto filename = std::string("read_test_")
            .append(argv[1])
            //.append(timestamp)
            .append(".csv");

    std::ofstream outfile(filename);
    outfile << "threads;time;" << std::endl;

    const int K = 5;

    for(int j = 1; j<= 8; j++) {

        std::cout << "THREADS: " << j << std::endl;

        for (int i = 0; i < K; i++) {
            (i*100)/K % 5 == 0 ? std::cout << (i*100)/K << "%" << std::endl : std::cout ;
            auto start_time = std::chrono::high_resolution_clock::now();
            loadFromFile(argv[1], j);
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            outfile << j << ";" << duration << ";" << std::endl;

        }
    }

    outfile.close();
}