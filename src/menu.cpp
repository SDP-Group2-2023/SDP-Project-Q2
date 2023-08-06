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
    cout << "To load graph - g " << endl;
    cout << "To partition graph - s (sequential), p (parallel), c (speed comparison)" << endl;
    cout << "To exit program -e " << endl;
    while(command!= 'e'){
        cin >> command;
        cout << "How many partitions should be found in graph?" << endl;
        cin >> requestedPartitions;

        switch (command) {
            case 'g' :
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
            case 's':
                cout << "Running sequential partitioning - (Multilevel KL)" << endl;
                partitioning(g, requestedPartitions);
                break;
            case 'p':
                cout << "Running parallel partitioning - ()" << endl;
                break;
            case 'c':
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
