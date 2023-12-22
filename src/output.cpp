#include "Graph.h"
#include "partitioning.h"
#include <fstream>
#include <vector>

/**
TODO
*/
void save_to_file(const std::string& path, const GraphPtr & graph,
                  const std::vector<unsigned int> &partitions,int requestedPartitions) {
    std::ofstream output_file(path);

    std::vector<unsigned int> partitions_sizes(requestedPartitions, 0);

    for (int i = 0; i < graph->V(); i++)
        partitions_sizes[partitions[i]] += graph->nodes[i]->weight;

    unsigned int max = 0;
    unsigned int min = graph->node_weight_global;
    unsigned int avg = 0;

    for (auto w : partitions_sizes) {
        if (w > max)
            max = w;
        if (w < min)
            min = w;
        avg += w;
    }

    avg /= requestedPartitions;

    
    for (int i = 0; i < graph->nodes.size(); i++)
        output_file << "Node: " << i << " in partition: " << partitions[i] << "." << std::endl;

    output_file << "END" << std::endl << std::endl;
    output_file << "Max partitions: " << max << " Min partition: " << min << " avg partitions " << avg << " expected average: " << graph->node_weight_global / requestedPartitions<< std::endl;
}