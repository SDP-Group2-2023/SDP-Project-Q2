#include "Graph.h"
#ifndef CHANGE_H
#define CHANGE_H

struct Change{
    Node* node;
    int new_partition;
    int gain;
    bool operator< (const Change &other);
};

#endif
