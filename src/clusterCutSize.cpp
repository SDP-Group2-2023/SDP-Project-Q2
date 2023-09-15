#include "clusterCutSize.h"

cluster_cut_size::cluster_cut_size(int clusterA, int clusterB, int cutSize) : clusterA(clusterA), clusterB(clusterB), cutSize(cutSize) {
    if (clusterA > clusterB){
        this->clusterA = clusterB;
        this->clusterB = clusterA;
    }
}

cluster_cut_size::cluster_cut_size() : clusterA(-1), clusterB(-1), cutSize(-1) {}

bool cluster_cut_size::operator<(const cluster_cut_size &other) const {
    return (this->cutSize < other.cutSize) || (this->cutSize == other.cutSize && this->clusterA > other.clusterA) ||
           (this->clusterA == other.clusterA && this->cutSize == other.cutSize && this->clusterB > other.clusterB);
}
