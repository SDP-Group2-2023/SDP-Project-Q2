#include "Change.h"
#include "Graph.h"
#include "timing/timing.h"
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
    return cutsize / 2;
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
    int cut_size      = calculateCutSize(graph, partitions);
    int best_cut_size = cut_size;
    vector<int> best_partitions(partitions);

    // keep track of the partitions weights (we added this anticipated calculation to improve performance timing,
    // indeed changes are proportional to V*P (nodes*partitions) and recomputing partition weight is proportional
    // to V --> complexity of for loop on possible_changes was P*V^2. With this change complexity in worst case remains V*P
    // indeed algorithm becomes slower from iteration 4 instead of 8 now with the improvement
    if (graph->partitions_size == nullptr) {
        graph->partitions_size =
            new vector<int>(num_partitions);    // this could possible be made a graph attribute, so to speed up, since it doesn't change from iteration to iteration
        for (int i = 0; i < num_partitions; i++)
            (*(graph->partitions_size))[i] = countPartitionWeight(graph, i, partitions);
    }
    vector<int> best_partitions_weights((*graph->partitions_size));

    // set<change> changes; // possibili cambi
    set<Change> possible_changes;
    // map<Node*, map<int, int>> // other
    map<Node *, map<int, int>> node_gain_mapping;

    // int cutsize_current_bestPartition;
    // int cutsize_current_Partition;
    // int prev_cutsize_difference = cutsize_current_Partition-cutsize_current_bestPartition

    timing choices_loop;
    // loop to calculate all initial gains
    for (int i = 0; i < graph->V; i++) {
        Node *current_node = graph->nodes[i];

        for (int p = 0; p < num_partitions; p++) {    // assign node to all possible partitions other than his
            if (p != partitions[current_node->id]) {
                possible_changes.emplace(p, current_node, gain(graph, partitions, current_node, p));
                node_gain_mapping[current_node][p] = gain(graph, partitions, current_node, p);
            }
        }
    }
    choices_loop.stop();

    cout << "Total time spent creating choices: " << choices_loop.getDuration() << endl;

    set<Change> best_possible_changes(possible_changes);
    map<Node *, map<int, int>> best_node_gain_mapping(node_gain_mapping);

    do {
        improved = false;
        // constraint (see article explanation : each node must be moved only once in the innermost loop
        vector<bool> moved(graph->V, false);

        // stopping criterion (page 7 of A Parallel Graph Partitioning algorithm for a message passing multiprocessor)
        int stop_threshold = -graph->max_node_degree();
        int sum_of_gains   = 0;
        int tot_moves      = 0;
        int negative_gains = 0;

        int num_iteration = 0;
        int iteration, max_iteration = 0, avg_iteration = 0;
        timing choosing_loop(TIMING_DEFER);

        timing change_node(TIMING_DEFER);
        timing change_neighbors(TIMING_DEFER);
        timing copying(TIMING_DEFER);

        bool flag = true;

        while (sum_of_gains >= stop_threshold && negative_gains < graph->max_node_degree()) {
            // from the set select the best (if leads to balanced partitions) gain movement and perform it (update partitions vector)
            Change best_change;
            iteration = 0;
            choosing_loop.start();
            for (auto &c : possible_changes) {    // consider the possibility of removing or not adding some possible changes to speed up subsequent iterations
                iteration++;
                if ((*graph->partitions_size)[partitions[c.node->id]] >= graph->node_weight_global / num_partitions &&
                    (*graph->partitions_size)[c.new_partition] <= graph->node_weight_global / num_partitions && !moved[c.node->id] && c.gain != 0) {
                    best_change       = c;
                    moved[c.node->id] = true;
                    break;
                }
            }
            choosing_loop.stop();

            if (iteration > max_iteration)
                max_iteration = iteration;
            avg_iteration += iteration;

            // if no change satisfies swapping criteria exit loop
            if (best_change.new_partition == -1)
                break;

            // increment moves for this cycle
            tot_moves++;

            // if the gain is negative increment the counter, otherwise reset it
            if (best_change.gain <= 0) {
                negative_gains++;
                flag = false;
            } else
                negative_gains = 0;

            // update the weights
            (*graph->partitions_size)[partitions[best_change.node->id]] -= best_change.node->weight;
            (*graph->partitions_size)[best_change.new_partition] += best_change.node->weight;

            // swap according to best change found
            int old_partition                = partitions[best_change.node->id];
            partitions[best_change.node->id] = best_change.new_partition;
            // possible_changes.erase(best_change);
            //  managing stopping criterion
            sum_of_gains += best_change.gain;
            // update necessary gain values in the set (neighbours)
            // no longer useful since every node is moved only once in one iteration of inner loop
            change_node.start();
            for (int i = 0; i < num_partitions; i++) {
                // removing selected change from possible changes set
                // if (i != partitions[best_change.node->id]) {
                Change new_change;
                new_change.node          = best_change.node;
                new_change.new_partition = i;
                // if (i != old_partition) {
                new_change.gain = node_gain_mapping[new_change.node][i];

                possible_changes.erase(new_change);
                //}
                // update gain of selected node to all other partitions and insert change in set
                // new_change.gain = gain(graph, partitions, new_change.node, i);
                // possible_changes.insert(new_change);
                // update node gain mapping for further references
                // node_gain_mapping[new_change.node][i] = gain(graph, partitions, new_change.node, i);
                //}
            }
            change_node.stop();
            // update gains of all neighbouring nodes of new_change.node
            change_neighbors.start();
            for (auto n : best_change.node->get_neighbors()) {
                int partition = old_partition;
                for (int i = 0; i < 2; i++) {
                    // removing selected change from possible_changes set (remember that
                    // to remove elements from a set all details of the element must be provided to erase function)
                    // if (i != partitions[n->id]) {
                    Change new_change;
                    new_change.node          = n;
                    new_change.new_partition = partition;

                    new_change.gain = node_gain_mapping[n][partition];
                    possible_changes.erase(new_change);
                    // update gain of selected node to all other partitions and insert change in set
                    new_change.gain = gain(graph, partitions, n, partition);
                    possible_changes.insert(new_change);
                    // update node gain mapping for further references
                    node_gain_mapping[n][partition] = gain(graph, partitions, n, partition);

                    partition = best_change.new_partition;
                    //}
                }
            }
            change_neighbors.stop();

            // update the cut size
            cut_size -= best_change.gain;

            if (cut_size < best_cut_size) {
                copying.start();
                best_partitions         = partitions;
                improved                = true;
                best_cut_size           = cut_size;
                best_partitions_weights = (*graph->partitions_size);
                if (!flag) {
                    best_node_gain_mapping = node_gain_mapping;
                    best_possible_changes  = possible_changes;
                    flag                   = true;
                }
                copying.stop();
            }
            num_iteration++;
        }

        // if best != current
        //      assign the best to current (partitions)
        //      improved = true;

        copying.start();
        partitions                = best_partitions;
        cut_size                  = best_cut_size;
        (*graph->partitions_size) = best_partitions_weights;
        if (!flag) {
            possible_changes  = best_possible_changes;
            node_gain_mapping = best_node_gain_mapping;
        }
        copying.stop();

        cout << "With " << tot_moves << " moves, we moved the cut size to " << cut_size << endl;
        cout << "last iterations: " << iteration << " max_iteration: " << max_iteration << " avg_iteration " << avg_iteration / num_iteration << endl;
        cout << "total time spent making choices: " << choosing_loop.getDuration() << endl;
        cout << "Total time spent copying data " << copying.getDuration() << " Total time spent on changing moved node possibilities " << change_node.getDuration()
             << " Total time spent changing neighbors: " << change_neighbors.getDuration() << endl;

    } while (improved);
}
