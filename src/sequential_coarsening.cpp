#include "partitioning.h"

/**
 * @brief Sort nodes by weight
 * @param nodes
 * @return sorted vector of nodes
 */
std::vector<Node*> sortNodes(const std::vector<Node*>& nodes) {
    std::vector<Node *> sortedNodes = nodes;
    sort(sortedNodes.begin(), sortedNodes.end(),
         [](Node *n1, Node *n2) {
        return n1->weight < n2->weight;
    });
    return sortedNodes;
}

/**
 * @brief Sort edges by weight
 * @param edges
 * @return sorted vector of edges
 */
std::vector<std::shared_ptr<Edge>> sortEdge(const std::vector<std::shared_ptr<Edge>>& edges) {
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
std::shared_ptr<Graph> coarseGraph_s(std::shared_ptr<Graph>& originalGraph){
    std::shared_ptr<Graph> coarse_graph(new Graph());
    int index = 0;
    std::vector<bool> matchedNodes(originalGraph->V(), false);

    auto orderedNodes = sortNodes(originalGraph->nodes);
    for(auto&n: orderedNodes){
        if(matchedNodes[n->id])
            continue;

        auto sortedEdges = sortEdge(n->edges);
        for(auto&e :sortedEdges){
            if(!matchedNodes[e->node1->id] && !matchedNodes[e->node2->id]){
                Node*n1 = e->node1;
                Node*n2 = e->node2;
                Node*newNode = coarse_graph->add_node(index, n1->weight + n2->weight);
                n1->child =  n2->child = newNode;
                matchedNodes[n1->id] = true;
                matchedNodes[n2->id] = true;
                index++;
                break;
            }
        }

        if(!matchedNodes[n->id]){
            Node*newNode = coarse_graph->add_node(index, n->weight);
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