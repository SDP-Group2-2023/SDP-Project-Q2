#include <iostream>
#include "../partitioning.h"
#include "../timing/timing.h"

/**
 * Funzione che si occupa di testare la velocità media di partizionamento di un grafo in maniera sequenziale
 * e la qualità delle partizioni
 * @param argc numero di argomenti passati
 * @param argv il primo param è il nome del programma, il secondo è il path del file
 */
int main(int argc, char**argv){
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " <input file>"  << std::endl;
        return 1;
    }

    auto g = loadFromFile(argv[1]);

    const int K = 5;

    long long timings = 0;
    long long min = LONG_LONG_MAX;
    long long max = LONG_LONG_MIN;

    for(int i = 0; i< K; i++) {

        //(i*100)/K % 5 == 0 ? std::cout << (i*100)/K << "%" << std::endl : std::cout ;

        auto start_time = std::chrono::high_resolution_clock::now();

        partitioning_s(g, 100);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        timings += duration;
        if(duration > max) max = duration;
        if(duration < min) min = duration;
    }

    std::cout << "MIN: " << min << std::endl;
    std::cout << "MAX: " << max << std::endl;
    std::cout << "AVG: " << timings/K << std::endl;

}