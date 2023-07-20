#include <iostream>
#include <fstream>

using namespace std;
int main(int argc, char *argv[]){
    if(argc < 3){
        cout << "Usage: " << argv[0] << " <input file> <output file>"  << endl;
        return 1;
    }

    ifstream input(argv[1]);
    ofstream output(argv[2], ios::binary );

    char line[1024];
    input.getline(line, 1024);input.getline(line, 1024);
    input.getline(line, 1024);input.getline(line, 1024);
    string a; int num_nodes, num_edges; char endline;
    input >> a >> a >> num_nodes >> num_edges >> endline;
    output.write((char*)&num_nodes, sizeof(int));
    output.write((char*)&num_edges, sizeof(int));
    //cout << num_nodes << " " << num_edges << endl;
    input.getline(line, 1024);
    input.getline(line, 1024);

    int partenza, destinazione, distanza;
    for(int i = 1; i<=num_nodes; i++){
        output.write((char*)&i, sizeof(int));
        int random = rand()%1000;
        output.write((char*)&random, sizeof(int));
    }

    for(int i = 0; i<num_edges; i++){
        input >> a >> partenza >> destinazione >> distanza;
        //cout << partenza << " " << destinazione << " " << distanza << endl;
        output.write((char*)&partenza, sizeof(int));
        output.write((char*)&destinazione, sizeof(int));
        output.write((char*)&distanza, sizeof(int));
    }

    return 0;
}