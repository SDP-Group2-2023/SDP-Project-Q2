#ifndef CLUSTER_CUT_SIZE
#define CLUSTER_CUT_SIZE
struct cluster_cut_size{
    cluster_cut_size(unsigned int clusterA,unsigned int clusterB, int cutSize);
    cluster_cut_size();
    unsigned int clusterA;
    unsigned int clusterB;
    int cutSize;
    bool operator<(const cluster_cut_size &other) const;
};
#endif