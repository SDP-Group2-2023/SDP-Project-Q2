#include "partitioning.h"

/**
 * @brief Sort nodes by weight
 * @param nodes
 * @return sorted vector of nodes
 */
std::vector<NodePtr> sortNodes( std::vector<NodePtr>& nodes) {
    auto sortedNodes = nodes;
    sort(sortedNodes.begin(), sortedNodes.end(),
         [](const NodePtr& n1, const NodePtr& n2) {
        return n1->weight < n2->weight;
    });
    return sortedNodes;
}

/**
 * @brief Sort edges by weight
 * @param edges
 * @return sorted vector of edges
 */
std::vector<EdgePtr> sortEdge( std::vector<EdgePtr>& edges) {
    auto sortedEdges = edges;
    sort(sortedEdges.begin(), sortedEdges.end(),
         [](auto e1, auto e2) {
             return e1->weight > e2->weight;
         });
    return sortedEdges;
}

/**
 * @brief Coarsen the graph based on SHEM algorithm
 * @param originalGraph
 * @return coarsened graph
 */
GraphPtr coarseGraph_s(GraphPtr & originalGraph){
    auto coarse_graph = std::make_shared<Graph>();
    int index = 0;
    std::vector<bool> matchedNodes(originalGraph->V(), false);

    auto orderedNodes = sortNodes(originalGraph->nodes);
    for(auto&n: orderedNodes){
        if(matchedNodes[n->id])
            continue;

        auto sortedEdges = sortEdge(n->edges);
        for(auto&e :sortedEdges){
            if(!matchedNodes[e->node1->id] && !matchedNodes[e->node2->id]){
                auto n1 = e->node1;
                auto n2 = e->node2;
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

    for(auto& e : originalGraph->edges){
        if(e->node1->child != e->node2->child)
            coarse_graph->add_or_sum_edge(e->node1->child, e->node2->child, e->weight);
    }

    return coarse_graph;
}