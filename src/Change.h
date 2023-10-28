#include "Graph.h"
#ifndef CHANGE_H
#define CHANGE_H

struct Change{
    Change(int new_partition, std::shared_ptr<Node> node, int gain);
    Change();
    std::shared_ptr<Node> node;
    int new_partition;
    int gain;
    bool operator< (const Change &other) const;
};

#endif
