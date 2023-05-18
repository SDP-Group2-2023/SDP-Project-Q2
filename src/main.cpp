#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

  if(argc != 2) {
    cout << "error: invalid arguments" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  if(!file.is_open()) {
    cout << "error: file not found" << endl;
    return 1;
  }

  // leggo tutti i vertici
  int N;
  file >> N;
  for (int i = 0; i<N; i++) {
    int nodeCode, nodeWeight;
    file >> nodeCode >> nodeWeight;
    cout << nodeCode << " " << nodeWeight << endl;
  }

  // leggo tutti gli archi
  int nodeCodeA, nodeCodeB, edgeWeight;
  while(file >> nodeCodeA >> nodeCodeB >> edgeWeight) {
    cout << nodeCodeA << " " << nodeCodeB << " " << edgeWeight << endl;
  }

    return 0;
}

