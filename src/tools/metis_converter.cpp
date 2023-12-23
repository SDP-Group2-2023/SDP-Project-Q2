#include <fstream>
#include <iostream>
#include <string>
#include "../partitioning.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage:" << std::endl << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    std::cout << "Loading from file" << std::endl;
    auto graph = loadFromFile(std::string(argv[1]), 4);

    std::ofstream out(argv[2]); // open the file in text mode

    std::cout << "Writing file" << std::endl;
    out << graph->V() << " " << graph->E() << " 011" << std::endl;

    for (auto node : graph->nodes) {
        out << node->weight;
        for (auto edge : node->edges) {
            out << " ";
            auto node1 = edge->node1.lock();

            if (node1->id != node->id)
                out << node1->id + 1;
            else
                out <<edge->node2.lock()->id + 1;

            out << " " << edge->weight;
        }
        out << std::endl;
    }

    out.close();
}
