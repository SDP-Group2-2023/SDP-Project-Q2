#include "Change.h"
#include "Graph.h"
#include <iostream>
#include <map>
#include <set>

/**
 * @brief Calculates the weight of the given partition
 * @param graph the graph
 * @param partition the partition for which you want to calculate the weight
 * @param partitions the vector that contains the partition number for each node
 * @return the weight of the partition calculated as the sum of all the weights of the nodes that belong to it
*/
unsigned int countPartitionWeight(const GraphPtr& graph, int partition, std::vector<unsigned int> &partitions) {
    unsigned int weight = 0;
    for (const auto &n : graph->nodes) {
        if (partitions[n->id] == partition) {
            weight += n->weight;
        }
    }
    return weight;
}

/**
 * @brief calculates the cut size of the whole graph among all partitions
 * @param graph the graph
 * @param partitions the vector that associates at each node its partition number
 * @return the cut size of the graph with the current partitioning
*/
unsigned long long calculateCutSize(const GraphPtr& graph, std::vector<unsigned int> &partitions) {
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

/**
 * @brief calculates the gain or the reduction in cut size that is achieved by moving the node to the specified partition
 * @param partitions the vector that assigns to each node its partition
 * @param node_to_move the pointer to the node that we would want to move
 * @param to_partition the partition we would like to move the node to
 * @return the gain that would be obtained by moving the node to the partition
*/
int gain(std::vector<unsigned int> &partitions, const NodePtr& node_to_move, int to_partition) {
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

/**
 * @brief Fiduccia and Mattheyses version KL-inspired was used to implement the kernighanLin function
 * @param graph the graph
 * @param num_partitions the total number of partitions
 * @param partitions the vector that will contain the refined partitioning after the execution of the algorithm
*/
void kernighanLin(const GraphPtr & graph, int num_partitions, std::vector<unsigned int> &partitions) {
    bool improved;
    unsigned long long cut_size      = calculateCutSize(graph, partitions);
    unsigned long long best_cut_size = cut_size;
    std::vector<unsigned int> best_partitions(partitions);

    /* Partition weight calculation has been anticipated outside the do-while in order to improve timing performance:
     * indeed "possible_changes" computation is proportional to V*P (number of nodes*number of partitions) and recomputing
     * partitions' weights is proportional to V --> the complexity of the for loop (on the possible_changes) was P*V^2 quadratic in V
     * With such anticipation the worst case complexity is V*P, linear in V.
     * We now observe the algorithm bottleneck is moved from "iteration 8" to "iteration 4", which is our new bottleneck, which
     * we are trying to resolve.
     */
    graph->partitions_size = std::vector<int>(num_partitions);  
    for (int i = 0; i < num_partitions; i++)
        graph->partitions_size[i] = countPartitionWeight(graph, i, partitions);

    std::vector<int> best_partitions_weights(graph->partitions_size);

    do {
        improved = false;

        // constraint (see article explanation : 
        //each node must be moved only once inside the innermost loop so we mark it with a flag)
        std::vector<bool> moved(graph->V(), false);
        std::set<Change> possible_changes;
        std::map<NodePtr, std::map<unsigned int, int>> node_gain_mapping;

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

        // stopping criterion 
        int stop_threshold = -graph->max_node_degree();
        int sum_of_gains   = 0;
        int tot_moves      = 0;
        int negative_gains = 0;

        int num_iteration = 0;
        int iteration, max_iteration = 0, avg_iteration = 0;
    

        while (sum_of_gains >= stop_threshold && negative_gains < graph->max_node_degree()) {
            // from the set select the best (if it leads to balanced partitions) gain movement and perform it (update "partitions" vector)
            Change best_change;
            iteration = 0;
            
            for (auto &c : possible_changes) {    
                iteration++;
                if (graph->partitions_size[partitions[c.node->id]] >= graph->node_weight_global / num_partitions &&
                    graph->partitions_size[c.new_partition] <= graph->node_weight_global / num_partitions && !moved[c.node->id] && c.gain != 0) {
                    best_change       = c;
                    moved[c.node->id] = true;
                    break;
                }
            }
            

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
            unsigned int old_partition                = partitions[best_change.node->id];
            partitions[best_change.node->id] = best_change.new_partition;
            possible_changes.erase(best_change);

            // managing stopping criterion
            sum_of_gains += best_change.gain;

            // update gains of all neighbouring nodes of new_change.node
            for (const auto& n : best_change.node->get_neighbors()) {
                for (const auto& neighbour : n->get_neighbors()) {
                    unsigned int i = partitions[neighbour->id];
                    // removing selected change from possible_changes set

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

        

    } while (improved);


}
