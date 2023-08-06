#include "Change.h"
#include "Graph.h"
#include <iostream>
#include <map>
#include <set>

using namespace std;

int countPartitionWeight(Graph *graph, int partition, vector<int> &partitions) {
    int weight = 0;
    for (auto &n : graph->nodes) {
        if (partitions[n->id] == partition) {
            weight += n->weight;
        }
    }
    return weight;
}

long calculateCutSize(Graph *graph, vector<int> &partitions) {
    long cutsize = 0;
    for (auto &n : graph->nodes) {
        for (auto &edge : n->edges) {
            int source, dest;
            source = edge->node1->id;
            dest   = edge->node2->id;
            if (partitions[source] != partitions[dest])
                cutsize += edge->weight;
        }
    }
    return cutsize;
}

int gain(Graph *graph, vector<int> &partitions, Node *node_to_move, int to_partition) {
    if (partitions[node_to_move->id] == to_partition)
        return 0;

    int result = 0;
    for (auto &e : node_to_move->edges) {
        Node *other = (e->node1 == node_to_move) ? e->node2 : e->node1;
        if (partitions[other->id] == to_partition)
            result += e->weight;
        else if (partitions[other->id] == partitions[node_to_move->id])
            result -= e->weight;
    }

    return result;
}

// Fiduccia and Mattheyses version KL-inspired
// we use a set and a double hash map the structure:

void kernighanLin(Graph *graph, int num_partitions, vector<int> &partitions) {
    bool improved;
    do {
        improved = false;
        vector<int> best_partitions(partitions);
        //constraint (see article explanation : each node must be moved only once in the innermost loop
        vector<bool> moved(graph->V, false);
        // set<change> changes; // possibili cambi
        set<Change> possible_changes;
        // map<Node*, map<int, int>> // other
        map<Node *, map<int, int>> node_gain_mapping;

        // int cutsize_current_bestPartition;
        // int cutsize_current_Partition;
        // int prev_cutsize_difference = cutsize_current_Partition-cutsize_current_bestPartition

        // loop to calculate all initial gains
        int swap_allowed = true;
        for (int i = 0; i < graph->V; i++) {
            Node *current_node = graph->nodes[i];

            for (int p = 0; p < num_partitions; p++) {    // assign node to all possible partitions other than his

                if (p != partitions[current_node->id]) {
                    possible_changes.emplace(p, current_node, gain(graph, partitions, current_node, p));
                    node_gain_mapping[current_node][p] = gain(graph, partitions, current_node, p);
                }
            }
        }
        // stopping criterion (page 7 of A Parallel Graph Partitioning algorithm for a message passing multiprocessor)
        int stop_threshold = -graph->max_node_degree();
        int sum_of_gains   = 0;


        while (sum_of_gains < stop_threshold) {
            // from the set select the best (if leads to balanced partitions) gain movement and perform it (update partitions vector)
            Change best_change;
            best_change.new_partition = -1;
            for (auto &c : possible_changes) {
                if (countPartitionWeight(graph, partitions[c.node->id], partitions) >= graph->node_weight_global / num_partitions &&
                    countPartitionWeight(graph, c.new_partition, partitions) <= graph->node_weight_global / num_partitions &&
                    !moved[c.node->id]) {
                    best_change = c;
                    moved[c.node->id] = true;
                    break;
                }
            }
            //if no change satisfies swapping criteria exit loop
            if(best_change.new_partition == -1)
                break;



            // swap according to best change found
            // int old_partition                = partitions[best_change.node->id];
            partitions[best_change.node->id] = best_change.new_partition;
            possible_changes.erase(best_change);
            // managing stopping criterion
            sum_of_gains += best_change.gain;
            // update necessary gain values in the set (neighbours)
            // no longer useful since every node is moved only once in one iteration of inner loop
            /*for (int i = 0; i < num_partitions; i++) {
                // removing selected change from possible changes set
                if (i != partitions[best_change.node->id]) {
                    Change new_change;
                    new_change.node          = best_change.node;
                    new_change.new_partition = i;
                    if (i != old_partition) {
                        new_change.gain = node_gain_mapping[new_change.node][i];

                        possible_changes.erase(new_change);
                    }
                    // update gain of selected node to all other partitions and insert change in set
                    new_change.gain = gain(graph, partitions, new_change.node, i);
                    possible_changes.insert(new_change);
                    // update node gain mapping for further references
                    node_gain_mapping[new_change.node][i] = gain(graph, partitions, new_change.node, i);
                }
            }*/
            // update gains of all neighbouring nodes of new_change.node
            for (auto n : best_change.node->get_neighbors()) {
                for (int i = 0; i < num_partitions; i++) {
                    // removing selected change from possible_changes set (remember that
                    // to remove elements from a set all details of the element must be provided to erase function)
                    if (i != partitions[n->id]) {
                        Change new_change;
                        new_change.node          = n;
                        new_change.new_partition = i;

                        new_change.gain = node_gain_mapping[n][i];
                        possible_changes.erase(new_change);
                        // update gain of selected node to all other partitions and insert change in set
                        new_change.gain = gain(graph, partitions, n, i);
                        possible_changes.insert(new_change);
                        // update node gain mapping for further references
                        node_gain_mapping[n][i] = gain(graph, partitions, n, i);
                    }
                }
            }
            if (calculateCutSize(graph, partitions) < calculateCutSize(graph, best_partitions)) {
                best_partitions = partitions;
                improved        = true;
            }
        }

        // if best != current
        //      assign the best to current (partitions)
        //      improved = true;

        partitions = best_partitions;
    } while (!improved);
}
