#include <iostream>
#include <fstream>
#include <random>

void skip_lines(std::ifstream& input, int num_lines);

int main(int argc, char *argv[]){
    if(argc < 3){
        std::cout << "Usage: " << argv[0] << " <input file> <output file>"  << std::endl;
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution distribution(1, 1000);

    std::ifstream input(argv[1]);
    if(!input.is_open()){
        std::cout << "Error opening file " << argv[1] << std::endl;
        return 1;
    }

    std::ofstream output(argv[2], std::ios::binary );
    if(!output.is_open()){
        std::cout << "Error opening file " << argv[2] << std::endl;
        return 1;
    }
    std::cout << "Reading from " << argv[1] << " and writing to " << argv[2] << std::endl;
    skip_lines(input, 4);

    std::string useless_chars;
    unsigned int num_nodes;
    unsigned long num_edges;
    char end_line;
    input >> useless_chars >> useless_chars
    >> num_nodes >> num_edges >> end_line;

    num_edges /= 2;
    output.write((char*)&num_nodes, sizeof(unsigned int));
    output.write((char*)&num_edges, sizeof(unsigned long));
    //std::cout << num_nodes << " " << num_edges << std::endl;
    skip_lines(input, 2);

    int source;
    int dest;
    int distance;
    std::cout << "Writing nodes..." << std::endl;
    for(int i = 0; i<num_nodes; i++){
        output.write((char*)&i, sizeof(unsigned int));
        int random = distribution(gen);
        output.write((char*)&random, sizeof(unsigned int));
    }

    std::cout << "Writing edges..." << std::endl;
    for(int i = 0; i<num_edges; i++) {
        input >> useless_chars
        >> source >> dest >> distance >> end_line;
        source--;
        dest--;
        output.write((char *) &source, sizeof(unsigned int));
        output.write((char *) &dest, sizeof(unsigned int));
        output.write((char *) &distance, sizeof(unsigned int));
        skip_lines(input, 1);
    }

    std::cout << "Done!" << std::endl;
    input.close();
    output.close();

    return 0;
}

void skip_lines(std::ifstream& input, int num_lines){
    std::string line;
    for(int i = 0; i<num_lines; i++)
        getline(input, line);
}