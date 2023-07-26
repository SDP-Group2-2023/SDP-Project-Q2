#include <iostream>
#include <fstream>
#include <random>

using namespace std;
int main(int argc, char *argv[]){
    if(argc < 3){
        cout << "Usage: " << argv[0] << " <input file> <output file>"  << endl;
        return 1;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(1, 1000);

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

    char line[1024];
    input.getline(line, 1024);input.getline(line, 1024);
    input.getline(line, 1024);input.getline(line, 1024);
    string a; int num_nodes, num_edges; char end_line;
    input >> a >> a >> num_nodes >> num_edges >> end_line;
    output.write((char*)&num_nodes, sizeof(int));
    num_edges /= 2;
    output.write((char*)&num_edges, sizeof(int));
    cout << num_nodes << " " << num_edges << endl;

    input.getline(line, 1024);
    input.getline(line, 1024);

    int partenza, destinazione, distanza;
    cout << "Writing nodes..." << endl;
    for(int i = 1; i<=num_nodes; i++){
        output.write((char*)&i, sizeof(int));
        int random = distribution(gen);
        output.write((char*)&random, sizeof(int));
    }

    cout << "Writing edges..." << endl;
    for(int i = 0; i<num_edges; i++) {
        input >> a >> partenza >> destinazione >> distanza >> end_line;
        output.write((char *) &partenza, sizeof(int));
        output.write((char *) &destinazione, sizeof(int));
        output.write((char *) &distanza, sizeof(int));
        input.getline(line, 1024);
    }

    cout << "Done!" << endl;
    input.close();
    output.close();

    return 0;
}