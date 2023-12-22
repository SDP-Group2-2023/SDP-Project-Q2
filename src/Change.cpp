#include "Change.h"

Change::Change(unsigned int new_partition,const NodePtr& node, int gain) : new_partition(new_partition), node(node), gain(gain), valid(true) {}

Change::Change() : new_partition(-1), node(nullptr), gain(-1), valid(false) {}

bool Change::operator<(const Change &other) const {
    return (this->gain > other.gain) || (this->gain == other.gain && this->node > other.node) ||
           (this->node == other.node && this->gain == other.gain && this->new_partition > other.new_partition);
}
