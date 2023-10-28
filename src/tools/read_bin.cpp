
#include <iostream>
#include <fstream>

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

    unsigned int num_nodes;
    unsigned long num_edges;
    input.read((char*)&num_nodes, sizeof(unsigned int));
    input.read((char*)&num_edges, sizeof(unsigned long));
    cout << "num_nodes " << num_nodes << endl;
    cout << "num_edges " << num_edges << endl;
    output << num_nodes << " " << num_edges << endl;

    return 0;


    unsigned int node;
    unsigned int node_weight;
    for (int i = 0;  i<num_nodes ; i++) {
        input.read((char*)&node, sizeof(unsigned int));
        input.read((char*)&node_weight, sizeof(unsigned int));

        cout << node << " " << node_weight << endl;
        //output << node << " " << node_weight << endl;

        if(node > num_nodes-1) {
            cout << "Error: node " << node << " is greater than num_nodes " << num_nodes << endl;
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
            cout << "Error: source " << source << " is greater than num_nodes " << num_nodes << endl;
            return 1;
        }
        if(dest > num_nodes-1) {
            cout << "Error: dest " << dest << " is greater than num_nodes " << num_nodes << endl;
            return 1;
        }

        cout << source << " " << dest << " " << distance << endl;
        //output << source << " " << dest << " " << distance << endl;
    }
}
