
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


    unsigned int node;
    unsigned int node_weight;
    for (int i = 0;  i<num_nodes ; i++) {
        input.read((char*)&node, sizeof(unsigned int));
        input.read((char*)&node_weight, sizeof(unsigned int));

        //std::cout << node << " " << node_weight << std::endl;
        output << node << " " << node_weight << std::endl;

        if(node > num_nodes-1) {
            std::cout << "Error: node " << node << " is greater than num_nodes " << num_nodes << std::endl;
            return 1;
        }

    }


    unsigned int source;
    unsigned int dest;
    unsigned int distance;
    for(int i = 0; i<num_edges; i++){
        input.read((char*)&source, sizeof(unsigned int));
        input.read((char*)&dest, sizeof(unsigned int));
        input.read((char*)&distance, sizeof(unsigned int));

        if(source > num_nodes-1) {
            std::cout << "Error: source " << source << " is greater than num_nodes " << num_nodes << std::endl;
            return 1;
        }
        if(dest > num_nodes-1) {
            std::cout << "Error: dest " << dest << " is greater than num_nodes " << num_nodes << std::endl;
            return 1;
        }

        //std::cout << source << " " << dest << " " << distance << std::endl;
        output << source << " " << dest << " " << distance << std::endl;
    }
}
