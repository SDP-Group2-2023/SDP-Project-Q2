#include "Graph.h"
#include "clusterCutSize.h"
#include "partitioning.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>


unsigned int calculate_end_condition_s(unsigned int num_nodes, unsigned int num_partitions){
    return std::max(30*num_partitions, num_nodes/(40* (unsigned int)log2(num_partitions)));
}

struct RetrieveKey {
    template<typename T> typename T::first_type operator()(T keyValuePair) const {
        return keyValuePair.first;
    }
};

/**
 * Funzione che aggiorna le partizioni di un grafo padre basandosi sui valori del grafo figlio
 * @param g grafo padre
 * @param partitions vettore partizioni del grafo figlio
 * @return vettore partizioni del grafo padre
 */
std::vector<unsigned int> uncoarsenGraph(const GraphPtr& g, std::vector<unsigned int> &partitions) {
    std::vector<unsigned int> newPartitions(g->V());

    for (const auto &n : g->nodes)
        newPartitions[n->id] = partitions[n->child->id];

    return newPartitions;
}

/**
 * @brief Will partition the graph using a sequential algorithm
 * @param g the graph to partition
 * @param partitions the vector that weill contain the generated partitions
 * @param partition_num the number of partitions to generate
*/
void initial_partitioning_s(const GraphPtr& g, std::vector<unsigned int> &partitions, unsigned int partition_num) {
    auto partitions_tot = g->V();
    std::map<unsigned int, std::map<unsigned int, unsigned int>> cluster_hashMap;
    std::set<cluster_cut_size> cut_sizes;

    for (int i = 0; i < g->V(); i++)   // first assign each node to its own partition
        partitions[i] = i;

    for (const auto& e : g->edges) {    // then populate the set and the hashmap
        cluster_cut_size cluster(e->node1.lock()->id, e->node2.lock()->id, e->weight);
        cut_sizes.insert(cluster);
        cluster_hashMap[cluster.clusterA][cluster.clusterB] = cluster.cutSize;
        cluster_hashMap[cluster.clusterB][cluster.clusterA] = cluster.cutSize;
    }
    while (partitions_tot > partition_num) {
        bool flag = false;
        partitions_tot--;
        cluster_cut_size selected = *cut_sizes.begin();    // select the topmost element
        cut_sizes.erase(selected);
        for (unsigned int & partition : partitions) {
            if (partition == selected.clusterB)
                {partition = selected.clusterA;
                flag           = true;
            }
        }

        std::vector<int> keys;


        // Retrieve all keys
        // this function I copied from stack overflow to extract all the keys
        transform(cluster_hashMap[selected.clusterB].begin(), cluster_hashMap[selected.clusterB].end(), back_inserter(keys), RetrieveKey());
        
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

        cluster_hashMap[selected.clusterA].erase(selected.clusterB);    // remove cluster B from A
        cluster_hashMap.erase(selected.clusterB);    // I remove cluster B
    }

    std::map<int, int> converter;
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
        std::cout << "Error in the partitioning" << std::endl;    // To be removed once testing is done
}

/**
 * @brief will partition the given graph
 * @param g the graph to partition
 * @param requestedPartitions the number of partitions to generate
*/
std::vector<unsigned int>  partitioning_s(const GraphPtr& g, int requestedPartitions) {
    unsigned int actual_num_partitions = g->V();

    std::vector<GraphPtr> allGraphs;
    allGraphs.push_back(g);

    int iterations = 0;
    while (actual_num_partitions > calculate_end_condition_s(g->V(), requestedPartitions)
        && iterations++ < 50) {
        auto coarsedGraph = coarseGraph_s(allGraphs.back());
        actual_num_partitions = coarsedGraph->V();
        allGraphs.push_back(coarsedGraph);
    }

    auto coarsestGraph = allGraphs[allGraphs.size() - 1];
    std::vector<unsigned int> partitions(coarsestGraph->V());
    initial_partitioning_s(coarsestGraph, partitions, requestedPartitions);

    kernighanLin(coarsestGraph, requestedPartitions, partitions);

    for (auto i = (int) allGraphs.size() - 2; i >= 0; i--) {
        partitions = uncoarsenGraph(allGraphs[i], partitions);
        allGraphs[i]->partitions_size = allGraphs[i + 1]->partitions_size;
        kernighanLin(allGraphs[i], requestedPartitions, partitions);
    }

    return partitions;

    //save_to_file("OutputPartitions.txt", g, partitions, requestedPartitions);

}
