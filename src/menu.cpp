#include <iostream>
#include <list>
#include "partitioning.h"

int main() {

    int requestedPartitions;
    char command = 's';
    std::string path;
    GraphPtr g = nullptr;

    int numThreads;

    do {
        std::cout << "Please select action:" << std::endl;
        std::cout << "G - load graph " << std::endl;
        std::cout << "S - sequential partitioning " << std::endl;
        std::cout << "P - parallel partitioning " << std::endl;
        std::cout << "C - sequential vs parallel comparison " << std::endl;
        std::cout << "E - exit program " << std::endl;
        std::cin >> command;
        command = (char) toupper(command);
        std::cout << "Selected option: " << command << std::endl;
        switch (command) {
            case 'G' :
                try {
                    if (g == nullptr) {
                        std::cout << "Enter graph path" << std::endl;
                        char tmp;
                        std::cin >> tmp;
                        getline(std::cin, path);
                        g = loadFromFile(path);
                    } else {
                        bool option = false;
                        std::cout << "Graph already loaded, overwrite it? (true or false) " << std::endl;
                        std:: cin >> option;
                        if (option) {
                            std::cout << "Enter graph path" << std::endl;
                            getline(std::cin, path);
                            g = loadFromFile(path);
                        }
                    }
                }
                catch (std::runtime_error &e) {
                    std::cout << e.what() << std::endl << std::endl;
                }
                break;
            case 'S':
                if (g == nullptr) {
                    std::cout << "Please load graph first" << std::endl;
                    break;
                }
                std::cout << "Running sequential partitioning - (Multilevel KL)" << std::endl;
                std::cout << "How many partitions should be found in graph?" << std::endl;
                std::cin >> requestedPartitions;
                partitioning_s(g, requestedPartitions);
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
                partitioning_p(g, requestedPartitions, numThreads);
                break;
            case 'C':
                if (g == nullptr) {
                    std::cout << "Please load graph first" << std::endl;
                    break;
                }
                //get timing to run sequential
                // get timing to run parallel
                //compare
                break;
            case 'E':
                std::cout << "Exiting program" << std::endl;
                break;
            default:
                std::cout << "Please select a valid option\n" << std::endl;
                break;
        }
    } while (command != 'E');

    //delete g;
}