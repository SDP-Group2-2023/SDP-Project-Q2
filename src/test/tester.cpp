#include <iostream>
#include <fstream>
#include "../partitioning.h"
#include "../timing/timing.h"


void calculatePartitionStats(const std::vector<int> &partition_size, unsigned long *min_partition, unsigned long *max_partition,
                             unsigned long *avg_partition, unsigned long *median_partition, unsigned long long* std_dev_partition){
    *min_partition = partition_size[0];
    *max_partition = partition_size[0];
    *avg_partition = 0;
    for(int i = 1; i < partition_size.size(); i++){
        *avg_partition += partition_size[i];
        if(partition_size[i] < *min_partition)
            *min_partition = partition_size[i];
        if(partition_size[i] > *max_partition)
            *max_partition = partition_size[i];
    }
    *avg_partition /= partition_size.size();
    *std_dev_partition = 0;

    for(int i : partition_size){
        *std_dev_partition += (i - *avg_partition) * (i - *avg_partition);
    }

    *std_dev_partition /= partition_size.size();
    *std_dev_partition = sqrt(*std_dev_partition);

    std::vector<int> partition_size_copy = std::vector(partition_size);
    std::sort(partition_size_copy.begin(), partition_size_copy.end());
    *median_partition = partition_size_copy[partition_size_copy.size()/2];

}

/**
 * Funzione che si occupa di testare la velocità di lettura media di un grafo
 * al variare del numero di thread usati
 * @param argc numero di argomenti passati
 * @param argv il primo param è il nome del programma, il secondo è il path del file
 */
void read_test(std::string& graph_path, int num_iterations){

    std::cout << "READ TEST" << std::endl;

    auto filename = std::string("read_test_")
            .append(graph_path);
    filename = filename.substr(0, filename.find_last_of('.')).append(".csv");

    std::ofstream outfile(filename, std::ios::out);

    if(!outfile.is_open()){
        std::cout << "Error opening file" << std::endl;
        return;
    }

    outfile << "threads,time" << std::endl;

    for(int j = 1; j<= 8; j++) {

        std::cout << "THREADS: " << j << std::endl;

        for (int i = 0; i < num_iterations; i++) {

            (i*100)/num_iterations % 5 == 0 ? std::cout << (i*100)/num_iterations << "%" << std::endl : std::cout ;

            auto start_time = std::chrono::high_resolution_clock::now();
            auto g = loadFromFile(graph_path, j);

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

            outfile << j << "," << duration << std::endl;

        }

        std::cout << "DONE" << std::endl;
    }

    outfile.close();

    std::cout << "TEST COMPLETED" << std::endl << std::endl;
}

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
            .append(graph_path);
    filename = filename.substr(0, filename.find_last_of('.')).append(".csv");

    std::ofstream outfile(filename, std::ios::out);
    if(!outfile.is_open()){
        std::cout << "Error opening file" << std::endl;
        return;
    }

    outfile << "time,cutsize,min_partition,max_partition,avg_partition,median_partition,std_dev" << std::endl;

    for(int i = 0; i< num_iterations; i++) {

        (i*100)/num_iterations % 5 == 0 ? std::cout << (i*100)/num_iterations << "%" << std::endl : std::cout ;

        auto start_time = std::chrono::high_resolution_clock::now();

        auto partitions = partitioning_s(g, 100);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        //calcolo dei valori statistici

        unsigned long long cutsize = calculateCutSize(g,partitions);
        unsigned long min_partition;
        unsigned long max_partition;
        unsigned long avg_partition;
        unsigned long median_partition;
        unsigned long long std_dev;

        calculatePartitionStats(g->partitions_size, &min_partition, &max_partition, &avg_partition, &median_partition, &std_dev);

        /*
        std::cout << "cutsize " << cutsize << std::endl;
        std::cout << "min: " << min_partition << " max: " << max_partition << std::endl;
        std::cout << "avg: " << avg_partition << " median: " << median_partition << std::endl;
        std::cout << "std_dev: " << std_dev << std::endl;
        */

        outfile << duration << "," << cutsize << "," ;
        outfile << min_partition << "," << max_partition << "," << avg_partition << ",";
        outfile << median_partition << "," << std_dev << std::endl;

    }

    outfile.close();

    std::cout << "TEST COMPLETED" << std::endl << std::endl;
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
            .append(graph_path);
    filename = filename.substr(0, filename.find_last_of('.')).append(".csv");

    std::ofstream outfile(filename, std::ios::out);
    if(!outfile.is_open()){
        std::cout << "Error opening file" << std::endl;
        return;
    }

    outfile << "threads,time,cutsize,min_partition,max_partition,avg_partition,median_partition,std_dev" << std::endl;

    for(int j = 1; j <= 8; j++) {

        std::cout << "THREADS: " << j << std::endl;

        for (int i = 0; i < num_iterations; i++) {

            (i * 100) / num_iterations % 5 == 0 ? std::cout << (i * 100) / num_iterations << "%" << std::endl : std::cout;

            auto start_time = std::chrono::high_resolution_clock::now();

            std::vector<unsigned int> partitions = partitioning_p(g, 100, j);

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

            //calcolo dei valori statistici

            unsigned long long cutsize = calculateCutSize(g,partitions);
            unsigned long min_partition;
            unsigned long max_partition;
            unsigned long avg_partition;
            unsigned long median_partition;
            unsigned long long std_dev;

            calculatePartitionStats(g->partitions_size, &min_partition, &max_partition,
                                    &avg_partition, &median_partition, &std_dev);

            /*
            std::cout << "cutsize " << cutsize << std::endl;
            std::cout << "min: " << min_partition << " max: " << max_partition << std::endl;
            std::cout << "avg: " << avg_partition << " median: " << median_partition << std::endl;
            std::cout << "std_dev: " << std_dev << std::endl;
            */

            outfile << j << "," << duration << "," << cutsize << "," ;
            outfile << min_partition << "," << max_partition << "," << avg_partition << ",";
            outfile << median_partition << "," << std_dev  << std::endl;


        }

        std::cout << "DONE" << std::endl;
    }

    outfile.close();

    std::cout << "TEST COMPLETED" << std::endl << std::endl;
}


int main(int argc, char**argv){
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " <input file> opt:<num_iterations>"  << std::endl;
        return 1;
    }

    const int num_iterations = argc==3 ? atoi(argv[2]) : 1;

    std::string graph_path(argv[1]);

    read_test(graph_path, num_iterations);
    seq_test(graph_path, num_iterations);
    par_test(graph_path, num_iterations);

}