
#include <iostream>
#include <fstream>

using namespace std;
int main(int argc, char**argv){
    if(argc < 2){
        cout << "Usage: " << argv[0] << " <input file>"  << endl;
        return 1;
    }
    fstream input(argv[1], ios::binary | ios::in);
    if(!input.is_open()){
        cout << "Error opening file " << argv[1] << endl;
        return 1;
    }

    ofstream output("temp.txt", ios::out);
    if(!output.is_open()){
        cout << "Error opening file temp.txt" << endl;
        return 1;
    }

    int num_nodes;
    int num_edges;
    input.read((char*)&num_nodes, sizeof(int));
    input.read((char*)&num_edges, sizeof(int));
    cout << "num_nodes" << num_nodes << endl;
    cout << "num_edges" << num_edges << endl;
    output << num_nodes << " " << num_edges << endl;

    int node;
    int node_weight;
    for (int i = 0;  i<num_nodes ; i++) {
        input.read((char*)&node, sizeof(int));
        input.read((char*)&node_weight, sizeof(int));
        cout << node << " " << node_weight << endl;
        output << node << " " << node_weight << endl;
    }

    int source;
    int dest;
    int distance;
    for(int i = 0; i<num_edges; i++){
        input.read((char*)&source, sizeof(int));
        input.read((char*)&dest, sizeof(int));
        input.read((char*)&distance, sizeof(int));
        cout << source << " " << dest << " " << distance << endl;
        output << source << " " << dest << " " << distance << endl;
    }
}
