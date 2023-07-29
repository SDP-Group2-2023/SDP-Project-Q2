#include <iostream>
#include <fstream>
#include <random>

using namespace std;

void skip_lines(ifstream& input, int num_lines);

int main(int argc, char *argv[]){
    if(argc < 3){
        cout << "Usage: " << argv[0] << " <input file> <output file>"  << endl;
        return 1;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution distribution(1, 1000);

    ifstream input(argv[1]);
    if(!input.is_open()){
        cout << "Error opening file " << argv[1] << endl;
        return 1;
    }

    ofstream output(argv[2], ios::binary );
    if(!output.is_open()){
        cout << "Error opening file " << argv[2] << endl;
        return 1;
    }
    cout << "Reading from " << argv[1] << " and writing to " << argv[2] << endl;
    skip_lines(input, 4);

    string useless_chars;
    int num_nodes;
    int num_edges;
    char end_line;
    input >> useless_chars >> useless_chars
    >> num_nodes >> num_edges >> end_line;

    num_edges /= 2;
    output.write((char*)&num_nodes, sizeof(int));
    output.write((char*)&num_edges, sizeof(int));
    cout << num_nodes << " " << num_edges << endl;
    skip_lines(input, 2);

    int source;
    int dest;
    int distance;
    cout << "Writing nodes..." << endl;
    for(int i = 0; i<num_nodes; i++){
        output.write((char*)&i, sizeof(int));
        int random = distribution(gen);
        output.write((char*)&random, sizeof(int));
    }

    cout << "Writing edges..." << endl;
    for(int i = 0; i<num_edges; i++) {
        input >> useless_chars
        >> source >> dest >> distance >> end_line;
        source--;
        dest--;
        output.write((char *) &source, sizeof(int));
        output.write((char *) &dest, sizeof(int));
        output.write((char *) &distance, sizeof(int));
        skip_lines(input, 1);
    }

    cout << "Done!" << endl;
    input.close();
    output.close();

    return 0;
}

void skip_lines(ifstream& input, int num_lines){
    string line;
    for(int i = 0; i<num_lines; i++){
        getline(input, line);
    }
}