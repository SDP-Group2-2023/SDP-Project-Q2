#include <iostream>
#include <list>
#include "partitioning.h"

int main() {

    int requestedPartitions;
    char command = 's';
    string path;
    shared_ptr<Graph> g = nullptr;

    do {
        cout << "Please select action:" << endl;
        cout << "G - load graph " << endl;
        cout << "S - sequential partitioning " << endl;
        cout << "P - parallel partitioning " << endl;
        cout << "C - sequential vs parallel comparison " << endl;
        cout << "E - exit program " << endl;
        cin >> command;
        command = (char) toupper(command);
        cout << "Selected option: " << command << endl;
        switch (command) {
            case 'G' :
                try {
                    if (g == nullptr) {
                        cout << "Enter graph path" << endl;
                        char tmp;
                        cin >> tmp;
                        getline(cin, path);
                        g = loadFromFile(path);
                    } else {
                        bool option = false;
                        cout << "Graph already loaded, overwrite it? (true or false) " << endl;
                        cin >> option;
                        if (option) {
                            cout << "Enter graph path" << endl;
                            getline(cin, path);
                            g = loadFromFile(path);
                        }
                    }
                }
                catch (std::runtime_error &e) {
                    cout << e.what() << endl << endl;
                }
                break;
            case 'S':
                if (g == nullptr) {
                    cout << "Please load graph first" << endl;
                    break;
                }
                cout << "Running sequential partitioning - (Multilevel KL)" << endl;
                cout << "How many partitions should be found in graph?" << endl;
                cin >> requestedPartitions;
                partitioning_s(g, requestedPartitions);
                break;
            case 'P':
                if (g == nullptr) {
                    cout << "Please load graph first" << endl;
                    break;
                }
                cout << "Running parallel partitioning - ()" << endl;
                cout << "How many partitions should be found in graph?" << endl;
                cin >> requestedPartitions;
                break;
            case 'C':
                if (g == nullptr) {
                    cout << "Please load graph first" << endl;
                    break;
                }
                //get timing to run sequential
                // get timing to run parallel
                //compare
                break;
            case 'E':
                cout << "Exiting program" << endl;
                break;
            default:
                cout << "Please select a valid option\n" << endl;
                break;
        }
    } while (command != 'E');

    //delete g;
}