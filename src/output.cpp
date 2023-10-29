#include "Graph.h"
#include "partitioning.h"
#include <fstream>
#include <vector>


void save_to_file(const std::string& path, const GraphPtr & graph,
                  const std::vector<int> &partitions,int requestedPartitions) {
    std::ofstream output_file(path);

    std::vector<int> partitions_sizes(requestedPartitions, 0);

    for (int i = 0; i < graph->V(); i++)
        partitions_sizes[partitions[i]] += graph->nodes[i]->weight;

    int max = 0;
    int min = graph->node_weight_global;
    int avg = 0;

    for (auto w : partitions_sizes) {
        if (w > max)
            max = w;
        if (w < min)
            min = w;
        avg += w;
    }

    avg /= requestedPartitions;

    std::cout << "Max partitions: " << max << " Min partition: " << min << " avg partitions " <<
    avg << " expected average: " << graph->node_weight_global / requestedPartitions << std::endl;

    for (int i = 0; i < graph->nodes.size(); i++)
        output_file << "Node: " << i << " in partition: " << partitions[i] << "." << std::endl;

    output_file << "END" << std::endl << std::endl;
    output_file << "Max partitions: " << max << " Min partition: " << min << " avg partitions " << avg << " expected average: " << graph->node_weight_global / requestedPartitions
                << std::endl;
}