#include "Change.h"
#include "Graph.h"
#include "timing/timing.h"
#include <iostream>
#include <map>
#include <set>

int countPartitionWeight(const GraphPtr& graph, int partition, std::vector<int> &partitions) {
    int weight = 0;
    for (const auto &n : graph->nodes) {
        if (partitions[n->id] == partition) {
            weight += n->weight;
        }
    }
    return weight;
}

unsigned long long calculateCutSize(const GraphPtr& graph, std::vector<int> &partitions) {
    unsigned long long cutsize = 0;
    for (const auto &n : graph->nodes) {
        for (const auto &edge : n->edges) {
            unsigned int source;
            unsigned int dest;
            source = edge->node1.lock()->id;
            dest   = edge->node2.lock()->id;
            if (partitions[source] != partitions[dest])
                cutsize += edge->weight >> 1;    // divided by 2
        }
    }
    return cutsize;
}

int gain(std::vector<int> &partitions, const NodePtr& node_to_move, int to_partition) {
    if (partitions[node_to_move->id] == to_partition)
        return 0;

    int result = 0;
    for (const auto &e : node_to_move->edges) {
        auto other = (e->node1.lock() == node_to_move) ? e->node2 : e->node1;
        if (partitions[other.lock()->id] == to_partition)
            result += e->weight;
        else if (partitions[other.lock()->id] == partitions[node_to_move->id])
            result -= e->weight;
    }

    return result;
}

// Fiduccia and Mattheyses version KL-inspired was used to implement the kernighanLin function

void kernighanLin(const GraphPtr & graph, int num_partitions, std::vector<int> &partitions) {
    bool improved;
    unsigned long long cut_size      = calculateCutSize(graph, partitions);
    unsigned long long best_cut_size = cut_size;
    std::vector<int> best_partitions(partitions);

    /* Partition weight calculation has been anticipated outside the do-while in order to improve timing performance:
     * indeed "possible_changes" computation is proportional to V*P (number of nodes*number of partitions) and recomputing
     * partitions' weights is proportional to V --> the complexity of the for loop (on the possible_changes) was P*V^2 quadratic in V
     * With such anticipation the worst case complexity is V*P, linear in V.
     * We now observe the algorithm bottleneck is moved from "iteration 8" to "iteration 4", which is our new bottleneck, which
     * we are trying to resolve.
     */
    graph->partitions_size = std::vector<int>(num_partitions);    // this could possible be made a graph attribute, so to speed up, since it doesn't change from iteration to iteration
    for (int i = 0; i < num_partitions; i++)
        graph->partitions_size[i] = countPartitionWeight(graph, i, partitions);

    std::vector<int> best_partitions_weights(graph->partitions_size);

    do {
        improved = false;

        // constraint (see article explanation : each node must be moved only once inside the innermost loop so we mark it with a flag)
        std::vector<bool> moved(graph->V(), false);
        std::set<Change> possible_changes;
        std::map<NodePtr, std::map<int, int>> node_gain_mapping;

        timing choices_loop;

        // loop to calculate all initial gains
        for (int i = 0; i < graph->V(); i++) {
            auto current_node = graph->nodes[i];

            for (const auto& n : current_node->get_neighbors()) {    // assign node to all possible partitions other than his
                if (partitions[n->id] != partitions[current_node->id]) {
                    possible_changes.emplace(partitions[n->id], current_node, gain(partitions, current_node, partitions[n->id]));
                    node_gain_mapping[current_node][partitions[n->id]] = gain(partitions, ref(current_node), partitions[n->id]);
                }
            }
        }
        choices_loop.stop();

        // stopping criterion (page 7 of "A Parallel Graph Partitioning algorithm for a message passing multiprocessor" explains this)
        int stop_threshold = -graph->max_node_degree();
        int sum_of_gains   = 0;
        int tot_moves      = 0;
        int negative_gains = 0;

        int num_iteration = 0;
        int iteration, max_iteration = 0, avg_iteration = 0;
        timing choosing_loop(timing_flag::TIMING_DEFER);

        while (sum_of_gains >= stop_threshold && negative_gains < graph->max_node_degree()) {
            // from the set select the best (if it leads to balanced partitions) gain movement and perform it (update "partitions" vector)
            Change best_change;
            iteration = 0;
            choosing_loop.start();
            for (auto &c : possible_changes) {    // consider the possibility of removing or not adding some possible changes to speed up subsequent iterations
                iteration++;
                if (graph->partitions_size[partitions[c.node->id]] >= graph->node_weight_global / num_partitions &&
                    graph->partitions_size[c.new_partition] <= graph->node_weight_global / num_partitions && !moved[c.node->id] && c.gain != 0) {
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
            if (best_change.gain <= 0)
                negative_gains++;
            else
                negative_gains = 0;

            // update the weights
            graph->partitions_size[partitions[best_change.node->id]] -= best_change.node->weight;
            graph->partitions_size[best_change.new_partition] += best_change.node->weight;

            // swap according to best change found
            int old_partition                = partitions[best_change.node->id];
            partitions[best_change.node->id] = best_change.new_partition;
            possible_changes.erase(best_change);

            // managing stopping criterion
            sum_of_gains += best_change.gain;

            // update gains of all neighbouring nodes of new_change.node
            for (auto n : best_change.node->get_neighbors()) {
                for (auto neighbour : n->get_neighbors()) {
                    int i = partitions[neighbour->id];
                    // removing selected change from possible_changes set (remember that to remove elements from a set all details of the element must be provided to erase
                    // function)
                    if (i != partitions[n->id]) {
                        Change new_change;
                        new_change.node          = n;
                        new_change.new_partition = i;

                        if (node_gain_mapping.contains(n) && node_gain_mapping[n].contains(i)) {
                            new_change.gain = node_gain_mapping[n][i];
                            possible_changes.erase(new_change);
                        }

                        // update gain of selected node to all other partitions and insert change in set
                        new_change.gain = gain(partitions, n, i);
                        possible_changes.insert(new_change);
                        // update node gain mapping for further references
                        node_gain_mapping[n][i] = gain(partitions, n, i);
                    }
                }
            }

            // update the cut size
            cut_size -= best_change.gain;

            if (cut_size < best_cut_size) {
                best_partitions         = partitions;
                improved                = true;
                best_cut_size           = cut_size;
                best_partitions_weights = graph->partitions_size;
            }
            num_iteration++;
        }

        partitions             = best_partitions;
        cut_size               = best_cut_size;
        graph->partitions_size = best_partitions_weights;

        // cout << "With " << tot_moves << " moves, we moved the cut size to " << cut_size << endl;
        // cout << "last iterations: " << iteration << " max_iteration: " << max_iteration << " avg_iteration " << avg_iteration / num_iteration << endl;
        // cout << "Total time spent creating choices: " << choices_loop.getDuration().count() << " - total time spent making choices: " << choosing_loop.getDuration().count() <<
        // endl;

    } while (improved);

    std::cout << calculateCutSize(graph, partitions) << std::endl;
}
