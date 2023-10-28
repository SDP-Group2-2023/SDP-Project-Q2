#include <iostream>
#include <fstream>

int main(int argc, char**argv){
    if(argc < 3){
        cout << "Usage: " << argv[0] << " <input file> <output file>"  << endl;
        return 1;
    }

    ifstream input(argv[1], ios::in);
    ofstream output(argv[2], ios::binary);
    int num_nodes;
    int num_edges;
    input >> num_nodes >> num_edges;
    output.write((char*)&num_nodes, sizeof(int));
    output.write((char*)&num_edges, sizeof(int));

    int id, node_weight;
    for(int i = 0; i< num_nodes; i++){
        input >> id >> node_weight;
        output.write((char*)&id, sizeof(int));
        output.write((char*)&node_weight, sizeof(int));
    }

    int source, dest, distance;
    for(int i = 0; i<num_edges; i++){
        input >> source >> dest >> distance;
        output.write((char*)&source, sizeof(int));
        output.write((char*)&dest, sizeof(int));
        output.write((char*)&distance, sizeof(int));
    }

    input.close();
    output.close();

}