#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void loadGraph();
void unloadGraph();
void calculateSequential();
void calculateParallel();
void compareSolutions();

int main(int argc, char* argv[]) {
int choice;
  bool valid = true;

  do{
    cout << "1 - Load a graph from file" << endl;
    cout << "2 - Unload the graph" << endl;
    cout << "3 - Calculate a partition in a sequential way" << endl;
    cout << "4 - Calculate a partition in a parallel way" << endl;
    cout << "5 - Compare the sequential and parallel partition" << endl;
    cout << "6 - Exit" << "\n" << endl;
      cin >> choice;
      if(cin.bad() || cin.fail()) {
        cout << "Invalid choice" << endl;
        cin.clear(); cin.ignore(10000, '\n');
        continue;
        }
     switch (choice)
  {
  case 1:
    loadGraph();
    break;
  case 2:
    unloadGraph();
    break;
  case 3:
    calculateSequential();
    break;
  case 4:
    calculateParallel();
    break;
  case 5:
    compareSolutions();
    break;
  case 6:
    cout << "Good bye" << endl;
    break;
  default:
    cout << "Invalid choice" << endl;
    break;
  }
  } while (choice != 6);
      return 0;
}

void loadGraph() {
  cout << "Loading graph" << endl;
}

void unloadGraph() {
  cout << "Unloading graph" << endl;
}

void calculateSequential() {
  cout << "Calculating sequential" << endl;
}

void calculateParallel() {
  cout << "Calculating parallel" << endl;
}

void compareSolutions() {
  cout << "Comparing solutions" << endl;
}