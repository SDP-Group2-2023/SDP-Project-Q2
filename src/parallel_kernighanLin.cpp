#include "Change.h"
#include "Graph.h"
#include "partitioning.h"
#include <algorithm>
#include <barrier>
#include <mutex>
#include <random>
#include <set>
#include <thread>
#include <vector>

using namespace std;

barrier<> *color_barrier;
barrier<> *weight_barrier;

/*serve grafo, num_partizioni, partizioni, num_threads
colori, num_colors
0-dividi nodi in modo casuale tra tutti i thread
  si comincia da un colore: ogni thread controlla tra i propri nodi
1-Computano il gain per i nodi del colore x e quali tra questi spostare (
    se c'è un nodo di quel colore che conviene spostare,
    considerando quello con gain positivo o gain=0 ma che migliorano il balance)
2-Due nodi dello stesso colore non sono adiacenti quindi possiamo spostarli insieme: non essendo
adiacenti non si influenzano il gain a vicenda
3-attendi terminazione dei thread e ripeti con un altro colore*/

void thread_kernighanLin(Graph *graph, int num_partitions, vector<int> *partitions, int num_colors, vector<int> *colors, vector<int> *nodes) {
    bool stuff_done = true;
    while (stuff_done) {
        stuff_done = false;
        for (int color = 0; color < num_colors; color++) {
            // Calculate partitions weight
            vector<int> partitions_size(num_partitions, 0);
            for (auto n : graph->nodes)
                partitions_size[partitions->at(n->id)] += n->weight;
            weight_barrier->arrive_and_wait();

            vector<int> actual_nodes;
            for (auto n : *nodes) {
                if ((*colors)[n] == color)
                    actual_nodes.push_back(n);
            }

            // Calculate the gains for every node
            set<Change> possible_changes;

            for (auto n : actual_nodes) {
                Node *node = graph->nodes[n];
                for (auto neighbour : node->get_neighbors()) {
                    if (partitions->at(neighbour->id) != partitions->at(node->id)) {
                        int C_gain = gain(*partitions, node, partitions->at(neighbour->id));
                        if (C_gain >= 0)
                            possible_changes.emplace(partitions->at(neighbour->id), node, C_gain);
                    }
                }
            }

            set<int> moved;

            while (true) {
                // from the set select the best (if it leads to balanced partitions) gain movement and perform it (update "partitions" vector)
                Change best_change;
                for (auto &c : possible_changes) {    // consider the possibility of removing or not adding some possible changes to speed up subsequent iterations
                    if (partitions_size[(*partitions)[c.node->id]] >= graph->node_weight_global / num_partitions &&
                        partitions_size[c.new_partition] <= graph->node_weight_global / num_partitions && !moved.contains(c.node->id)) {
                        best_change = c;
                        stuff_done  = true;
                        moved.insert(c.node->id);
                        break;
                    }
                }

                // if no change satisfies swapping criteria exit loop
                if (best_change.new_partition == -1)
                    break;

                // update the weights
                partitions_size[(*partitions)[best_change.node->id]] -= best_change.node->weight;
                partitions_size[best_change.new_partition] += best_change.node->weight;

                // apply the change
                (*partitions)[best_change.node->id] = best_change.new_partition;
                possible_changes.erase(best_change);
            }

            color_barrier->arrive_and_wait();
        }
    }
    // da rivedere se fare così oppure no
    weight_barrier->arrive_and_drop();
    color_barrier->arrive_and_drop();
    return;
}

void parallel_kernighanLin(Graph *graph, int num_partitions, vector<int> &partitions, int num_colors, vector<int> &colors, int num_threads) {
    /*
        divisione dei nodi ai singoli threads
        creazione dei threads
        aspetta il termine dei thread
    */

    vector<vector<int>> nodes_per_thread(num_threads);
    color_barrier  = new barrier(num_threads);
    weight_barrier = new barrier(num_threads);

    vector<int> nodes(graph->nodes.size());
    for (int i = 0; i < nodes.size(); i++)
        nodes[i] = i;

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(nodes), std::end(nodes), rng);

    for (auto n : nodes) {
        nodes_per_thread[rand() % num_threads].push_back(n);
    }
    nodes.clear();

    vector<thread> kernighanLiners;
    for (int i = 0; i < num_threads; i++) {
        kernighanLiners.emplace_back(thread_kernighanLin, num_partitions, &partitions, num_colors, &colors, &nodes_per_thread[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        kernighanLiners[i].join();
    }

    delete color_barrier;
    delete weight_barrier;

    return;
}