#include "Graph.h"
#include "partitioning.h"
#include <iostream>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

unsigned int calculate_end_condition_p(unsigned int num_nodes, unsigned int num_partitions){
    return std::max(30*num_partitions, num_nodes/(40* (unsigned int)log2(num_partitions)));
}

void uncoarsen_graph_step(const GraphPtr& g, std::vector<unsigned int> &partitions,
                          std::vector<unsigned int> &newPartitions, int num_nodes, int start, int step) {
    int i = start;
    while (i < num_nodes) {
        newPartitions[g->nodes[i]->id] = partitions[g->nodes[i]->child->id];
        i += step;
    }
}

std::vector<unsigned int> uncoarsen_graph_p(const GraphPtr& g, std::vector<unsigned int> &partitions, int num_thread) {
    unsigned int num_nodes = g->V();
    std::vector<unsigned int> newPartitions(num_nodes);
    std::vector<std::thread> threads;

    for (int i = 0; i < num_thread; i++) {
        threads.emplace_back(uncoarsen_graph_step, ref(g), ref(partitions), ref(newPartitions), num_nodes, i, num_thread);
    }
    for (auto &t : threads) {
        t.join();
    }

    return newPartitions;
}

std::vector<unsigned int> partitioning_p(const GraphPtr &g, int requestedPartitions, int num_threads) {
    unsigned int actual_num_partitions = g->V();

    std::vector<GraphPtr> allGraphs;
    allGraphs.push_back(g);

    int iterations = 0;
    while (actual_num_partitions >calculate_end_condition_p(g->V(), requestedPartitions)
            && iterations++ < 50)
    {
        auto coarsedGraph = coarseGraph_p(allGraphs.back(), num_threads);
        actual_num_partitions = coarsedGraph->V();
        allGraphs.push_back(coarsedGraph);
    }

    auto coarsestGraph = allGraphs[allGraphs.size() - 1];
    std::vector<unsigned int> partitions(coarsestGraph->V());//, -1);
    for (int i = 0; i < coarsestGraph->V(); i++)
        partitions[i] = i % requestedPartitions;

    kernighanLin_p(coarsestGraph, requestedPartitions, partitions, coarsestGraph->num_colours, coarsestGraph->colours, num_threads);

    for (auto i = (int) allGraphs.size() - 2; i >= 0; i--) {
        partitions = uncoarsen_graph_p(allGraphs[i], partitions, num_threads);
        allGraphs[i]->partitions_size = allGraphs[i + 1]->partitions_size;
        kernighanLin_p(allGraphs[i], requestedPartitions, partitions, coarsestGraph->num_colours, coarsestGraph->colours, num_threads);
    }

    return partitions;
}

/**
 * @brief compare two nodes and return true if the first has more weight than the second, or if the have the same weight it returns true if the first has a higher id
 * @param a the first node
 * @param b the second node
 * @return true if a goes before b, false otherwise
*/
bool Compare_Node(const NodePtr &a, const NodePtr &b) {
    if (a->weight > b->weight || (a->weight == b->weight && a->id > b->id))
        return true;
    else
        return false;
}

/**
 * @brief worker thread for the initial partitioning of the graph
 * @param graph the graph to partition
 * @param partitions the vector that will contain the final partitions of each node
 * @param nodes_m vector of mutexes for each node
 * @param starting_nodes vector of nodes assigned to this thread from where to begin
 * @param avg_p_weight the average partitions weight for a partition
 * @param weight vector where the weights of the generated partitions will be stored
*/
void partitioning_thread(GraphPtr &graph, std::vector<int>& partitions, std::vector<std::mutex>& nodes_m, std::vector<int>& starting_nodes, int avg_p_weight,
                         std::vector<int>& weight) {
    // vector of sets
    std::vector<std::set<NodePtr, bool (*)(const NodePtr &,const NodePtr &)>> queues_of_nodes;
    for (int starting_node : starting_nodes) {
        // collecting in a set all neighbours of my starting node that are still unassigned to a partition
        std::set<NodePtr, bool (*)(const NodePtr &, const NodePtr &)> queue(Compare_Node);
        for (const auto &n : graph->nodes[starting_node]->get_neighbors())
            queue.insert(n);

        queues_of_nodes.push_back(queue);
    }
    // we assign each neighbour to the partition of the starting node iff
    // the weight of this partition we are filling up is lower than the graph avg partition weight
    // otherwise we leave the neighbour unassigned and will treat it later
    bool next_partition = false;
    int index           = 0;
    // weight of the partition we are forming by adding each neighbour to it
    while (!next_partition) {
        for (auto neighbour = queues_of_nodes[index].begin(); neighbour != queues_of_nodes[index].end();) {
            // lock starting node and its neighbour
            auto sl = std::scoped_lock(nodes_m[(*neighbour)->id], nodes_m[starting_nodes[index]]);
            // we set 1.4 waiting to test if it's ok as a multiplication factor
            // if adding the neighbour to the partition does not exceed the avg and the neighbour hasn't been
            // assigned by another thread to another partition, assign it and remove it from the set
            // of unassigned node
            if (weight[index] + (*neighbour)->weight <= avg_p_weight * 1.4 && partitions[(*neighbour)->id] == -1) {
                partitions[(*neighbour)->id] = partitions[starting_nodes[index]];
                weight[index] += (*neighbour)->weight;

                auto s_neighbour = *neighbour;

                // add neighbour's neighbours to the queue
                for (auto &n : s_neighbour->get_neighbors())
                    queues_of_nodes[index].insert(n);

                // remove neighbour from unassigned
                queues_of_nodes[index].erase(s_neighbour);
                // reset iterator to the start of the set since we have added new neighbours to this set
                neighbour = queues_of_nodes[index].begin();

            } else if (partitions[(*neighbour)->id] != -1) {
                // if another thread assigned it already you can remove it from set of unassigned and release lock on it
                queues_of_nodes[index].erase(neighbour);
            } else {
                neighbour++;
            }
        }
        // move to next node in our starting pool
        index++;
        if (index > starting_nodes.size())
            next_partition = true;
    }
}

/**
 * @brief will perform a multithread partitioning of the graph
 * @param graph the graph to partition
 * @param partitions the vector where the final partitions will be saved
 * @param num_partitions number of desired partitions
 * @param num_threads threads to use for this operation
*/
void initial_partitioning_p(GraphPtr &graph, std::vector<int> &partitions, int num_partitions, int num_threads) {
    std::vector<std::vector<int>> starting_nodes(num_threads);
    std::set<NodePtr, bool (*)(const NodePtr &,const NodePtr &)> ordered_nodes(graph->nodes.begin(), graph->nodes.end(), Compare_Node);
    // one weight vector for each thread to operate on
    std::vector<std::vector<int>> weights(num_threads);

    auto top_node = ordered_nodes.begin();
    // more partitions than threads available: anyway this is just the initial partitioning
    for (int i = 0; i < num_partitions; i++) {
        partitions[(*top_node)->id] = i;
        starting_nodes[i % num_threads].push_back((*top_node)->id);
        // we save partition weights as we need to know them in the last step of this function: assignment of unassigned nodes
        weights[i % num_threads].push_back((*top_node)->weight);
        top_node++;
        if (top_node == ordered_nodes.end())
            break;
    }

    std::vector<std::mutex> nodes_m(graph->nodes.size());

    std::vector<std::thread> partitioners;

    for (int i = 0; i < num_threads; i++)
        partitioners.emplace_back(partitioning_thread, ref(graph), std::ref(partitions), ref(nodes_m),
                                  ref(starting_nodes[i]), graph->node_weight_global / num_partitions, ref(weights[i]));

    for (auto &t : partitioners)
        t.join();

    // assign leftover neighbouring nodes still without a partition
    // criterion: assign it to the LIGHTEST neighbouring partition
    for (int i = 0; i < partitions.size(); i++) {
        if (partitions[i] == -1) {
            int w_lightest_partition = -1;
            auto neighbours          = graph->nodes[i]->get_neighbors();
            for (const auto &s : neighbours) {
                if (partitions[s->id] == -1)
                    continue;
                if (w_lightest_partition == -1) {
                    w_lightest_partition = partitions[s->id];
                    continue;
                }
                if (weights[s->id % num_threads][s->id / num_threads] < weights[w_lightest_partition % num_threads][w_lightest_partition / num_threads]) {
                    w_lightest_partition = s->id;
                }
            }
            partitions[i] = partitions[w_lightest_partition];
            // if it crashes because w_lightest_partition == -1 still, try changing factor 1.4 above
        }
    }
}
