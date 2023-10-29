#include "Change.h"
#include "Graph.h"
#include "partitioning.h"
#include <algorithm>
#include <barrier>
#include <random>
#include <set>
#include <vector>
#include <thread>


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

void thread_kernighanLin(GraphPtr & graph, int num_partitions, std::vector<int> *partitions,
                         int num_colors, std::vector<int> *colors, std::vector<int> *nodes, std::barrier<>& weight_barrier, std::barrier<>& color_barrier) {
    bool stuff_done = true;
    int counter     = 0;
    while (stuff_done) {
        stuff_done = false;
        for (int color = 0; color < num_colors; color++) {
            // Calculate partitions weight
            std::vector<unsigned int> partitions_size(num_partitions, 0);
            for (auto& n : graph->nodes)
                partitions_size[partitions->at(n->id)] += n->weight;
            weight_barrier.arrive_and_wait();

            std::vector<int> actual_nodes;
            for (auto n : *nodes) {
                if ((*colors)[n] == color)
                    actual_nodes.push_back(n);
            }

            // Calculate the gains for every node
            std::set<Change> possible_changes;

            for (auto n : actual_nodes) {
                auto node = graph->nodes[n];
                for (auto& neighbour : node->get_neighbors()) {
                    if (partitions->at(neighbour->id) != partitions->at(node->id)) {
                        int C_gain = gain(*partitions, ref(node), partitions->at(neighbour->id));
                        if (C_gain >= 0)
                            possible_changes.emplace(partitions->at(neighbour->id), ref(node), C_gain);
                    }
                }
            }

            std::set<int> moved;

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

            color_barrier.arrive_and_wait();
        }
        if(!stuff_done && counter++ < 20)
            stuff_done = true;
    }
    // da rivedere se fare così oppure no
    weight_barrier.arrive_and_drop();
    color_barrier.arrive_and_drop();
}

void kernighanLin_p(GraphPtr & graph, int num_partitions, std::vector<int> &partitions, int num_colors, std::vector<int> &colors, int num_threads) {
    /*
        divisione dei nodi ai singoli threads
        creazione dei threads
        aspetta il termine dei thread
    */

    std::vector<std::vector<int>> nodes_per_thread(num_threads);
    std::barrier color_barrier(num_threads);
    std::barrier weight_barrier(num_threads);

    std::vector<int> nodes(graph->nodes.size());
    for (int i = 0; i < nodes.size(); i++)
        nodes[i] = i;

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(nodes), std::end(nodes), rng);

    for (auto n : nodes) {
        nodes_per_thread[rand() % num_threads].push_back(n);
    }
    nodes.clear();

    std::vector<std::thread> kernighanLiners;
    for (int i = 0; i < num_threads; i++) {
        kernighanLiners.emplace_back(thread_kernighanLin, ref(graph), num_partitions, &partitions, num_colors,
                                     &colors, &nodes_per_thread[i], std::ref(weight_barrier), std::ref(color_barrier));
    }

    for (int i = 0; i < num_threads; i++)
        kernighanLiners[i].join();


    std::cout << calculateCutSize(graph, partitions) << std::endl;
}
