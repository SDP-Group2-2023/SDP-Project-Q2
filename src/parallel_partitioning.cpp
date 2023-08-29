#include "Graph.h"
#include "partitioning.h"
#include <iostream>
#include <set>
#include <thread>
#include <mutex>
#include <vector>
using namespace std;

void uncoarsen_graph_step(Graph *g, vector<int> &partitions, vector<int> &newPartitions, int num_nodes, int start, int step) {
    int i = start;
    while (i < num_nodes) {
        newPartitions[g->nodes[i]->id] = partitions[g->nodes[i]->child->id];
        i += step;
    }
}

vector<int> uncoarsen_graph_p(Graph *g, vector<int> &partitions, int num_thread) {
    int num_nodes = g->V();
    vector<int> newPartitions(num_nodes);
    vector<thread> threads(num_thread);

    for (int i = 0; i < num_thread; i++) {
        threads[i] = thread(uncoarsen_graph_step, g, ref(partitions), ref(newPartitions), num_nodes, i, num_thread);
    }
    for (auto &t : threads) {
        t.join();
    }

    return newPartitions;
}

void partitioning_p(Graph *g, int requestedPartitions, int num_threads) {
    int actual_num_partitions = g->V();

    vector<Graph *> allGraphs;
    allGraphs.push_back(g);

    int iterations = 0;
    while (actual_num_partitions > requestedPartitions * 15 && iterations++ < 50) {
        cout << "Iteration " << iterations << endl;
        auto start          = chrono::high_resolution_clock::now();
        Graph *coarsedGraph = coarseGraph_p(allGraphs.back(), num_threads);
        auto end            = chrono::high_resolution_clock::now();
        cout << "Coarsening time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
        // coarsedGraph->print();
        actual_num_partitions = coarsedGraph->V();
        allGraphs.push_back(coarsedGraph);
    }

    Graph *coarsestGraph = allGraphs[allGraphs.size() - 1];
    vector<int> partitions(coarsestGraph->V(), -1);
    for (int i = 0; i < coarsestGraph->V(); i++) {
        partitions[i] = i % requestedPartitions;
    }

    kernighanLin(coarsestGraph, requestedPartitions, partitions);

    for (auto i = (int) allGraphs.size() - 2; i >= 0; i--) {
        partitions = uncoarsen_graph_p(allGraphs[i], partitions, num_threads);
        cout << "Uncoarsening step " << allGraphs.size() - i - 1 << endl;
        allGraphs[i]->partitions_size = allGraphs[i + 1]->partitions_size;
        kernighanLin(allGraphs[i], requestedPartitions, partitions);
    }

    for (int i = 0; i < partitions.size(); i++) {
        cout << "Node " << i << " in partition " << partitions[i] << endl;
    }

    for (int i = 1; i < allGraphs.size(); i++)
        delete allGraphs[i];
}

bool Compare_Node(Node *a, Node *b) {
    if (a->weight > b->weight || (a->weight == b->weight && a->id > b->id))
        return true;
    else
        return false;
}

void partitioning_thread(Graph *graph, vector<int> &partitions, vector<mutex> &nodes_m, vector<int> &starting_nodes, int avg_p_weight) {
    //vector of sets 
    vector<set<Node *, bool (*)(Node *, Node *)>> queues_of_nodes;
    for (int i = 0; i < starting_nodes.size(); i++) {
        //collecting in a set all neighbours of my starting node that are still unassigned to a partition
        set<Node *, bool (*)(Node *, Node *)> queue(Compare_Node);
        for(auto e: graph->nodes[starting_nodes[i]]->edges) {
            if(starting_nodes[i] == e->node1->id)
                queue.insert(e->node2);
            else
                queue.insert(e->node1);
        }

        queues_of_nodes.push_back(queue);
    }
    //we assign each neighbour to the partition of the starting node iff 
    //the weight of this partition we are filling up is lower than the graph avg partition weight
    //otherwise we leave the neighbour unassigned and will treat it later
    bool next_partition = false;
    int index = 0;
    //weight of the partition we are forming by adding each neighbour to it
    vector<int> weight(starting_nodes.size(), 0);
    while (!next_partition){

        std::unique_lock(nodes_m[starting_nodes[index]);

        for(auto neighbour: queues_of_nodes[index]){
            //lock starting node and its neighbour
            //std::scoped_lock(nodes_m[starting_nodes[index]], nodes_m[neighbour->id]);
            std::unique_lock(nodes_m[neighbour->id]);
                //we set 1.4 waiting to test if it's ok as a multiplication factor
                //if adding the neighbour to the partition does not exceed the avg and the neighbour hasn't been
                //assigned by another thread to another partition, assign it and remove it from the set
                //of unassigned node
                if (weight[index] + neighbour->weight <= avg_p_weight * 1.4 && partitions[neighbour->id] == -1) {
                    partitions[neighbour->id] = partitions[starting_nodes[index]];
                    weight[index] += neighbour->weight;

                    //remove neighbour from unassigned and release lock on it (mmh I can't release
                    //only the lock on the second if we use a scoped lock...)
                    std::set<int>::iterator it = queues_of_nodes[index].find(neighbour);
                    if (it != queues_of_nodes.end()) {
                        queues_of_nodes.erase(it);
                    }
                    //unlock neighbour??

                }else if(partitions[neighbour->id] != -1){
                    //if another thread assigned it already you can remove it from set of unassigned and release lock on it
                    std::set<int>::iterator it = queues_of_nodes[index].find(neighbour);
                    if (it != queues_of_nodes.end()) {
                        queues_of_nodes.erase(it);
                    }
                    //unlock neighbour??
                }else{
                    continue;
                }
        }
        //release lock on starting node...
        index++;
        if(index > starting_nodes.size()) next_partition = true;
    }

    //assign leftover neighbouring nodes still without a partition
    //criterion: assign it to the LIGHTEST neighbouring partition
    set<Node *> unassigned;
    for(int i = 0; i < partitions.size(); i++){
        if(partitions[i] == -1) unassigned.insert(graph->nodes[partitions[i]]);
    }

    int target_partition;
    int w_target_partition = 0;
    int w_lightest_partition = 0;

    for(auto node : unassigned){
        w_lightest_partition = 0;
        vector<Node *> neigh = node->get_neighbors();
        for(auto s : neigh){
            w_target_partition =  0; //function to compute partition size
            if (w_target_partition < w_lightest_partition)  {
                w_lightest_partition = w_target_partition;
                partitions[node->id] = partitions[neigh.id];}
        }
    }

}}

void initial_partitioning_p(Graph *graph, vector<int> &partitions, int num_partitions, int num_threads) {
    vector<vector<int>> starting_nodes(num_threads);
    set<Node *, bool (*)(Node *, Node *)> ordered_nodes(graph->nodes.begin(), graph->nodes.end(), Compare_Node);

    auto top_node = ordered_nodes.begin();
    // <3
    // more partitions than threads available: anyway this is just the initial partitioning
    for (int i = 0; i < num_partitions; i++) {
        partitions[(*top_node)->id] = i;
        starting_nodes[i % num_threads].push_back((*top_node)->id);
        top_node++;
    }

    vector<mutex> nodes_m(graph->nodes.size());
}
