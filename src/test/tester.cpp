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
void read_test(std::string& graph_path, int num_iterations){

    std::cout << "READ TEST" << std::endl;

    auto filename = std::string("read_test_")
            .append(graph_path)
            .append(".csv");

    std::ofstream outfile(filename);
    outfile << "threads;time;" << std::endl;

    for(int j = 1; j<= 8; j++) {

        std::cout << "THREADS: " << j;

        for (int i = 0; i < num_iterations; i++) {

            //(i*100)/num_iterations % 5 == 0 ? std::cout << (i*100)/num_iterations << "%" << std::endl : std::cout ;

            auto start_time = std::chrono::high_resolution_clock::now();
            auto g = loadFromFile(graph_path, j);

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

            //outfile << j << ";" << duration << ";" << std::endl;

        }

        std::cout << "*done*" << std::endl;
    }

    outfile.close();

    std::cout << "TEST COMPLETED" << std::endl;}

/**
 * Funzione che si occupa di testare la velocità media di partizionamento di un grafo in maniera sequenziale
 * e la qualità delle partizioni
 * @param argc numero di argomenti passati
 * @param argv il primo param è il nome del programma, il secondo è il path del file
 */
void seq_test(std::string& graph_path, int num_iterations){

    std::cout << "SEQUENTIAL PARTITIONING TEST" << std::endl;

    auto g = loadFromFile(graph_path);

    auto filename = std::string("seq_test_")
            .append(graph_path)
            .append(".csv");

    std::ofstream outfile(filename);
    outfile << "time;cutsize;min_partition;max_partition;avg_partition;median_partition;std_dev;" << std::endl;

    for(int i = 0; i< num_iterations; i++) {

        //(i*100)/num_iterations % 5 == 0 ? std::cout << (i*100)/num_iterations << "%" << std::endl : std::cout ;

        auto start_time = std::chrono::high_resolution_clock::now();

        partitioning_s(g, 100);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        //calcolo dei valori statistici

        long cutsize;
        long min_partition;
        long max_partition;
        long avg_partition;
        long median_partition;
        long std_dev;

        /*
        outfile << j << ";" << duration << ";" << cutsize << ";" ;
        outfile << min_partition << ";" << max_partition << ";" << avg_partition << ";";
        outfile << median_partition << ";" << std_dev << ";" << std::endl;
        */
    }

    outfile.close();

    std::cout << "TEST COMPLETED" << std::endl;
}

/**
* Funzione che si occupa di testare la velocità media di partizionamento di un grafo in maniera parallela
* e la qualità delle partizioni al variare del numero di thread usati
* @param argc numero di argomenti passati
* @param argv il primo param è il nome del programma, il secondo è il path del file
*/
void par_test(std::string& graph_path, int num_iterations){

    std::cout << "PARALLEL PARTITIONING TEST" << std::endl;

    auto g = loadFromFile(graph_path);

    auto filename = std::string("par_test_")
            .append(graph_path)
            .append(".csv");

    std::ofstream outfile(filename);
    outfile << "threads;time;cutsize;min_partition;max_partition;avg_partition;median_partition;std_dev;" << std::endl;

    for(int j = 1; j <= 8; j++) {

        std::cout << "THREADS: " << j;

        for (int i = 0; i < num_iterations; i++) {

            //(i * 100) / num_iterations % 5 == 0 ? std::cout << (i * 100) / num_iterations << "%" << std::endl : std::cout;

            auto start_time = std::chrono::high_resolution_clock::now();

            partitioning_p(g, 100, j);

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

            //calcolo dei valori statistici

            long cutsize;
            long min_partition;
            long max_partition;
            long avg_partition;
            long median_partition;
            long std_dev;

            /*
            outfile << j << ";" << duration << ";" << cutsize << ";" ;
            outfile << min_partition << ";" << max_partition << ";" << avg_partition << ";";
            outfile << median_partition << ";" << std_dev << ";" << std::endl;
            */

        }

        std::cout << "*done*" << std::endl;

    }

    outfile.close();

    std::cout << "TEST COMPLETED" << std::endl;
}


int main(int argc, char**argv){
    if(argc < 3){
        std::cout << "Usage: " << argv[0] << " <input file> <num_iterations>"  << std::endl;
        return 1;
    }

    const int num_iterations = atoi(argv[2]);

    std::string graph_path(argv[1]);

    read_test(graph_path, num_iterations);
    seq_test(graph_path, num_iterations);
    par_test(graph_path, num_iterations);

}