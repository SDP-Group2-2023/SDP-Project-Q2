#include <iostream>
#include <fstream>

using namespace std;
int main(int argc, char *argv[]){
    if(argc < 3){
        cout << "Usage: " << argv[0] << " <input file> <output file>"  << endl;
        return 1;
    }

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
    string a; int num_nodes, num_edges; char endline;
    input >> a >> a >> num_nodes >> num_edges >> endline;
    output.write((char*)&num_nodes, sizeof(int));
    output.write((char*)&num_edges, sizeof(int));
    cout << num_nodes << " " << num_edges << endl;
    input.getline(line, 1024);
    input.getline(line, 1024);

    int partenza, destinazione, distanza;
    cout << "Writing nodes..." << endl;
    for(int i = 1; i<=num_nodes; i++){
        output.write((char*)&i, sizeof(int));
        int random = rand()%1000+1;
        output.write((char*)&random, sizeof(int));
    }

    cout << "Writing edges..." << endl;
    for(int i = 0; i<num_edges; i++){
        input >> a >> partenza >> destinazione >> distanza;
        //cout << partenza << " " << destinazione << " " << distanza << endl;
        output.write((char*)&partenza, sizeof(int));
        output.write((char*)&destinazione, sizeof(int));
        output.write((char*)&distanza, sizeof(int));
    }

    cout << "Done!" << endl;
    input.close();
    output.close();
    return 0;
}