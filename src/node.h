class Node {
private:
  int weight;
  int id;
public:
  Node(int id, int weight);
  int getWeight();
};

Node::Node(int id, int weight) {
    this->id = id;
    this->weight = weight;
}

int Node::getWeight() {
    return weight;
}
