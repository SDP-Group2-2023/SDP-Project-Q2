#include "Node.h"

Node::Node(int id, int weight) {
    this->id = id;
    this->weight = weight;
}

int Node::getWeight() {
    return weight;
}