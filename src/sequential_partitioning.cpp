#include "Graph.h"
#include "clusterCutSize.h"
#include "partitioning.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;
struct RetrieveKey {
    template<typename T> typename T::first_type operator()(T keyValuePair) const {
        return keyValuePair.first;
    }
};

vector<int> uncoarsenGraph(Graph *g, vector<int> &partitions) {
    vector<int> newPartitions(g->V());

    for (auto &n : g->nodes)
        newPartitions[n->id] = partitions[n->child->id];

    return newPartitions;
}

void initial_partitioning_s(Graph *g, vector<int> &partitions, int partition_num) {
    int partitions_tot = g->V();
    map<int, map<int, int>> cluster_hashMap;
    set<cluster_cut_size> cut_sizes;

    for (int i = 0; i < g->V(); i++) {    // first assign each node to its own partition
        partitions[i] = i;
    }
    for (auto e : g->edges) {    // then populate the set and the hashmap
        cluster_cut_size cluster(e->node1->id, e->node2->id, e->weight);
        cut_sizes.insert(cluster);
        cluster_hashMap[cluster.clusterA][cluster.clusterB] = cluster.cutSize;
        cluster_hashMap[cluster.clusterB][cluster.clusterA] = cluster.cutSize;
    }
    while (partitions_tot > partition_num) {
        partitions_tot--;
        cluster_cut_size selected = *cut_sizes.begin();    // select the topmost element
        cut_sizes.erase(selected);
        for (int i = 0; i < partitions.size(); i++) {
            if (partitions[i] == selected.clusterB)
                partitions[i] = selected.clusterA;
        }
        vector<int> keys;

        // Retrieve all keys
        // this function I copied from stack overflow to extract all the keys
        transform(cluster_hashMap[selected.clusterB].begin(), cluster_hashMap[selected.clusterB].end(), back_inserter(keys), RetrieveKey());
        cluster_hashMap.erase(selected.clusterB);    // I remove cluster B
        for (auto key : keys) {                      // for each cluster that was connect to B
            if (key == selected.clusterA)
                continue;
            cluster_cut_size old(selected.clusterB, key, cluster_hashMap[selected.clusterB][key]);    // I remove the connection
            cut_sizes.erase(old);
            if (cluster_hashMap[key].contains(selected.clusterA)) {    // if it is also connected to A I update the cutsize between them in the hash map
                cluster_cut_size oldA(selected.clusterA, key, cluster_hashMap[selected.clusterA][key]);
                cut_sizes.erase(oldA);
                cluster_hashMap[key][selected.clusterA] += cluster_hashMap[key][selected.clusterB];
            } else {    // if it isn't I create the new connection and simply move over the cut size
                cluster_hashMap[key][selected.clusterA] = cluster_hashMap[key][selected.clusterB];
            }
            // Then I update the data also for cluster A
            cluster_hashMap[selected.clusterA][key] = cluster_hashMap[key][selected.clusterA];
            // then I remove B from the connected cluster
            cluster_hashMap[key].erase(selected.clusterB);
            cluster_cut_size new_C(selected.clusterA, key, cluster_hashMap[key][selected.clusterA]);
            cut_sizes.insert(new_C);
        }
    }

    map<int, int> converter;
    int partition = 0;
    for (int i = 0; i < partitions.size(); i++) {
        if (converter.contains(partitions[i]))
            partitions[i] = converter[partitions[i]];
        else {
            converter[partitions[i]] = partition;
            partitions[i]            = partition++;
        }
    }
    if (partition != partition_num)                     // should ALWAYS BE FALSE
        cout << "Error in the partitioning" << endl;    // To be removed once testing is done
}

void partitioning_s(Graph *g, int requestedPartitions) {
    int actual_num_partitions = g->V();

    vector<Graph *> allGraphs;
    allGraphs.push_back(g);

    int iterations = 0;
    while (actual_num_partitions > requestedPartitions * 15 && iterations++ < 50) {
        cout << "Iteration " << iterations << endl;
        Graph *coarsedGraph = coarseGraph_s(allGraphs.back());
        // coarsedGraph->print();
        actual_num_partitions = coarsedGraph->V();
        cout << "Actual number of partitions: " << actual_num_partitions << endl;
        allGraphs.push_back(coarsedGraph);
    }

    Graph *coarsestGraph = allGraphs[allGraphs.size() - 1];
    vector<int> partitions(coarsestGraph->V());
    initial_partitioning_s(coarsestGraph, partitions, requestedPartitions);

    kernighanLin(coarsestGraph, requestedPartitions, partitions);

    for (auto i = (int) allGraphs.size() - 2; i >= 0; i--) {
        partitions = uncoarsenGraph(allGraphs[i], partitions);
        cout << "Uncoarsening step " << i << endl;
        allGraphs[i]->partitions_size = allGraphs[i + 1]->partitions_size;
        kernighanLin(allGraphs[i], requestedPartitions, partitions);
    }

    for (int i = 0; i < partitions.size(); i++) {
        cout << "Node " << i << " in partition " << partitions[i] << endl;
    }

    for (int i = 1; i < allGraphs.size(); i++) {
        delete allGraphs[i];
    }
}
