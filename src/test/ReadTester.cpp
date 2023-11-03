#include <iostream>
#include <fstream>
#include "../partitioning.h"
#include "../timing/timing.h"

/**
 * Funzione che si occupa di testare la velocità di lettura media di un grafo
 * al variare del numero di thread usati
 * @param argc numero di argomenti passati
 * @param argv il primo param è il nome del programma, il secondo è il path del file
 */
int main(int argc, char**argv){
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " <input file>"  << std::endl;
        return 1;
    }

    auto filename = std::string("read_test_")
            .append(argv[1])
            .append(".csv");

    std::ofstream outfile(filename);
    outfile << "threads;time;" << std::endl;

    const int K = 20;

    for(int j = 1; j<= 8; j++) {

        std::cout << "THREADS: " << j << std::endl;

        for (int i = 0; i < K; i++) {
            (i*100)/K % 5 == 0 ? std::cout << (i*100)/K << "%" << std::endl : std::cout ;
            auto start_time = std::chrono::high_resolution_clock::now();
            auto g = loadFromFile(argv[1], j);

            //std::cout << g->V() << " " << g->E() << std::endl;

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            outfile << j << ";" << duration << ";" << std::endl;

        }
    }

    outfile.close();
}