#include <iostream>
#include <list>
#include "partitioning.h"

using namespace std;
int main(int argc, char **argv){

    int requestedPartitions;
    char command = 's';
    string path;
    Graph* g = nullptr;


    cout << "Please select action:" << endl;
    cout << "G - to load graph " << endl;
    cout << "S - sequential partitioning " << endl;
    cout << "P - parallel partitioning " << endl;
    cout << "C - sequential vs parallel comparison " << endl;
    cout << "To exit program -e " << endl;
    while(command!= 'e'){
        cin >> command;
        switch (command) {
            case 'G' :
                    if(g == nullptr){
                    cout << "Enter graph path" << endl;
                    char tmp;
                    cin >> tmp;
                    getline(cin, path);
                    g = loadFromFile(path);
                    }else{
                        bool option = false;
                        cout << "Graph already loaded, overwrite it? (true or false) " << endl;
                        cin >> option;
                        if(option){
                            cout << "Enter graph path" << endl;
                            getline(cin, path);
                            g = loadFromFile(path);
                        }}
                    break;
            case 'S':
                cout << "Running sequential partitioning - (Multilevel KL)" << endl;
                cout << "How many partitions should be found in graph?" << endl;
                cin >> requestedPartitions;
                partitioning(g, requestedPartitions);
                break;
            case 'P':
                cout << "Running parallel partitioning - ()" << endl;
                cout << "How many partitions should be found in graph?" << endl;
                cin >> requestedPartitions;
                break;
            case 'C':
                //get timing to run sequential
                // get timing to run parallel
                //compare
                break;
        }
    }
    delete g;
}//
// Created by utente on 06/08/2023.
//
