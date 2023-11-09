#include "partitioning.h"

/**
 * @brief Sort nodes by weight in ascending order
 * @param nodes
 * @return sorted vector of nodes
 */
NodePtrArr sortNodes(const NodePtrArr & nodes) {
    auto sortedNodes = nodes;

    std::ranges::sort(sortedNodes, [](const auto& n1, const auto& n2) {
        return n1->weight < n2->weight;
    });

    return sortedNodes;
}

/**
 * @brief Sort edges by weight in descending order
 * @param edges
 * @return sorted vector of edges
 */
EdgePtrArr sortEdge(const EdgePtrArr & edges) {
    auto sortedEdges = edges;

    std::ranges::sort(sortedEdges, [](const auto& e1, const auto& e2) {
        return e1->weight > e2->weight;
    });

    return sortedEdges;
}

/**
 * @brief Coarsen the graph based on SHEM algorithm
 * @param originalGraph
 * @return coarsened graph
 */
GraphPtr coarseGraph_s(const GraphPtr & originalGraph){
    auto coarse_graph = std::make_shared<Graph>();
    int index = 0;
    std::vector<bool> matchedNodes(originalGraph->V(), false);

    for(const auto&n: sortNodes(originalGraph->nodes)){
        if(matchedNodes[n->id])
            continue;

        for(const auto&e : sortEdge(n->edges)){
            if(!matchedNodes[e->node1.lock()->id] && !matchedNodes[e->node2.lock()->id]){
                auto n1 = e->node1.lock();
                auto n2 = e->node2.lock();
                auto newNode = coarse_graph->add_node(index, n1->weight + n2->weight);
                n1->child =  n2->child = newNode;
                matchedNodes[n1->id] = true;
                matchedNodes[n2->id] = true;
                index++;
                break;
            }
        }

        if(!matchedNodes[n->id]){
           auto newNode = coarse_graph->add_node(index, n->weight);
            n->child = newNode;
            index++;
        }
    }

    for(const auto& e : originalGraph->edges)
        if(e->node1.lock()->child != e->node2.lock()->child)
            coarse_graph->add_or_sum_edge(e->node1.lock()->child, e->node2.lock()->child, e->weight);

    return coarse_graph;
}