#include "Graph.h"
#include <fstream>
#include <vector>
#include <ctime>
#include <sstream>

/**
 * @brief save partitioning results to a file
 * @param graph
 * @param partitions
 * @param requestedPartitions
 */
void save_to_file(const GraphPtr & graph, const std::vector<unsigned int> &partitions,int requestedPartitions) {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%F-%T");
    std::string dateTimeString = oss.str();
    std::ofstream output_file(dateTimeString + ".txt");

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
        output_file << "Node " << i << " in partition " << partitions[i] << std::endl;

    output_file << std::endl;
    output_file << "Max partitions: " << max << std::endl;
    output_file << "Min partition: " << min << std::endl;
    output_file << "Avg partitions " << avg << std::endl;
    output_file << "Expected average: " << graph->node_weight_global / requestedPartitions<< std::endl;

    output_file.close();
    std::cout << "Result saved in file: " << dateTimeString << ".txt" << std::endl;
}