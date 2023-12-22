#include "Graph.h"
#ifndef CHANGE_H
#define CHANGE_H

struct Change{
    Change(int new_partition,const NodePtr& node, int gain);
    Change();
    unsigned int new_partition;
    bool valid;
    NodePtr node;
    int gain;
    bool operator< (const Change &other) const;
    Change(unsigned int new_partition, const NodePtr &node, int gain);
};

#endif
