#include "Change.h"

bool Change::operator< (const Change &other) {
    return (this->gain > other.gain) || ((this->gain == other.gain) && this->node > other.node);
}
