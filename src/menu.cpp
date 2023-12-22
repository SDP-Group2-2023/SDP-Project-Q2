#include <iostream>
#include <list>
#include <ctime>
#include "partitioning.h"

/**
TODO
*/
GraphPtr commandLoadFile(){
    GraphPtr g = nullptr;
    std::cout << "Enter graph path" << std::endl;
    std::string path;
    std::getline(std::cin, path);
    g = loadFromFile(path);

    if(g == nullptr) {
        std::cout << "Graph not loaded" << std::endl << std::endl;
    } else {
        std::cout << "Graph loaded" << std::endl << std::endl;
    }
    return g;
}
/**
TODO
*/
void partitioning_s_test(GraphPtr& g, int requestedPartitions){
    auto start_time_s = std::chrono::high_resolution_clock::now();
    partitioning_s(g, requestedPartitions);
    auto end_time_s = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_s - start_time_s).count();
    std::cout << "Sequential partitioning done in " << duration_s << " ms" << std::endl << std::endl;
}

/**
TODO
*/
void partitioning_p_test(GraphPtr& g, int requestedPartitions){
    for(int i = 2; i<= 8; i++){
        auto start_time_s = std::chrono::high_resolution_clock::now();
        partitioning_p(g, requestedPartitions, i);
        auto end_time_s = std::chrono::high_resolution_clock::now();
        auto duration_s = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_s - start_time_s).count();
        std::cout << "Parallel partitioning with " << i << " threads done in " << duration_s << " ms" << std::endl;
    }
}

int main() {

    int requestedPartitions;
    char command;// = 's';
    GraphPtr g = nullptr;
    std::vector<unsigned int> result;
    int numThreads;

    do {
        std::cout << "Please select action:" << std::endl;
        std::cout << "G - load graph " << std::endl;
        std::cout << "S - sequential partitioning " << std::endl;
        std::cout << "P - parallel partitioning " << std::endl;
        std::cout << "C - sequential vs parallel comparison " << std::endl;
        std::cout << "E - exit program " << std::endl;
        std::cin >> command;
        std::cin.ignore();
        command = (char) toupper(command);

        std::cout << "Selected option: " << command << std::endl;
        switch (command) {
            case 'G' :
                try {
                    if (g == nullptr) {
                        g = commandLoadFile();

                    } else {
                        char option;
                        std::cout << "Graph already loaded, overwrite it? (y/n) " << std::endl;
                        std:: cin >> option;
                        std::cin.ignore();
                        option = (char) toupper(option);
                        switch(option){
                            case 'Y':
                                g = commandLoadFile();
                                break;
                            case 'N':
                                std::cout << "Graph not overwritten" << std::endl << std::endl;
                                break;
                            default:
                                std::cout << "Invalid option" << std::endl << std::endl;
                                break;
                        }
                    }
                }
                catch (std::exception &e) {
                    std::cout << e.what() << std::endl << std::endl;
                }
                break;
            case 'S':
                if (g == nullptr) {
                    std::cout << "Please load graph first" << std::endl << std::endl;
                    break;
                }
                std::cout << "Running sequential partitioning - (Multilevel KL)" << std::endl;
                std::cout << "How many partitions should be found in graph?" << std::endl;
                std::cin >> requestedPartitions;
                try{
                result = partitioning_s(g, requestedPartitions);

                save_to_file(g, result, requestedPartitions);
                result.clear();
                std::cout << "Partitioning done" << std::endl << std::endl;
                }catch(std::runtime_error &e){
                        std::cout << e.what() << std::endl << std::endl;
                }
                break;
            case 'P':
                if (g == nullptr) {
                    std::cout << "Please load graph first" << std::endl;
                    break;
                }
                std::cout << "Running parallel partitioning - ()" << std::endl;
                std::cout << "How many partitions should be found in graph?" << std::endl;
                std::cin >> requestedPartitions;
                std::cout << "How many threads should be employed?" << std::endl;
                std::cin >> numThreads;
                try{
                result = partitioning_p(g, requestedPartitions, numThreads);
                save_to_file(g, result, requestedPartitions);
                result.clear();
                std::cout << "Partitioning done" << std::endl << std::endl;
                }catch(std::runtime_error &e){
                        std::cout << e.what() << std::endl << std::endl;
                }

                break;
            case 'C':
                if (g == nullptr) {
                    std::cout << "Please load graph first" << std::endl << std::endl;
                    break;
                }
                std::cout << "How many partitions should be found in graph?" << std::endl;
                std::cin >> requestedPartitions;

                std::cout << "Running sequential partitioning - (Multilevel KL)" << std::endl;
                partitioning_s_test(g, requestedPartitions);

                std::cout << "Running parallel partitioning - (Multilevel KL) - 2->8 threads" << std::endl;
                partitioning_p_test(g, requestedPartitions);
                std::cout << std::endl;
                break;
            case 'E':
                std::cout << "Exiting program" << std::endl;
                break;
            default:
                std::cout << "Please select a valid option\n" << std::endl;
                break;
        }
    } while (command != 'E');
}