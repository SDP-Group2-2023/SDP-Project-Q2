
#include <iostream>
#include <fstream>

int main(int argc, char**argv){
    if(argc < 2){
        std::cout << "Usage: " << argv[0] << " <input file>"  << std::endl;
        return 1;
    }
    std::fstream input(argv[1], std::ios::binary | std::ios::in);
    if(!input.is_open()){
        std::cout << "Error opening file " << argv[1] << std::endl;
        return 1;
    }

    std::ofstream output("temp.txt", std::ios::out);
    if(!output.is_open()){
        std::cout << "Error opening file temp.txt" << std::endl;
        return 1;
    }

    unsigned int num_nodes;
    unsigned long num_edges;
    input.read((char*)&num_nodes, sizeof(unsigned int));
    input.read((char*)&num_edges, sizeof(unsigned long));
    //std::cout << "num_nodes " << num_nodes << std::endl;
    //std::cout << "num_edges " << num_edges << std::endl;
    output << num_nodes << " " << num_edges << std::endl;

    return 0;

}
