#include "Graph.h"
#include "partitioning.h"
#include <fstream>
#include <vector>

using namespace std;

void save_to_file(string path, Graph *graph, const vector<int> &partitions, int requestedPartitions) {
    ofstream output_file(path);

    vector<int> partitions_sizes(requestedPartitions, 0);

    for (int i = 0; i < graph->V(); i++) {
        partitions_sizes[partitions[i]] += graph->nodes[i]->weight;
    }

    int max = 0, min = graph->node_weight_global, avg = 0;
    for (auto w : partitions_sizes) {
        if (w > max)
            max = w;
        if (w < min)
            min = w;
        avg += w;
    }

    avg /= requestedPartitions;

    cout << "Max partitions: " << max << " Min partition: " << min << " avg partitions " << avg << " expected average: " << graph->node_weight_global / requestedPartitions << endl;

    for (int i = 0; i < graph->nodes.size(); i++)
        output_file << "Node: " << i << " in partition: " << partitions[i] << "." << endl;

    output_file << "END" << endl << endl;
    output_file << "Max partitions: " << max << " Min partition: " << min << " avg partitions " << avg << " expected average: " << graph->node_weight_global / requestedPartitions
                << endl;
}