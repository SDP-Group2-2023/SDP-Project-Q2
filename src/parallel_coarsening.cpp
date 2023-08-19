#include "partitioning.h"
#include <future>

mutex m_node, m_edge;

void operation(Graph*coarse_graph, Node*n1, Node*n2, int index){
    m_node.lock();
    Node*newNode = coarse_graph->add_node(index, n1->weight + n2->weight);
    n1->child =  n2->child = newNode;
    m_node.unlock();
}

void operation2(Graph*coarse_graph, Edge*e){
    m_edge.lock();
    coarse_graph->add_or_sum_edge(e->node1->child, e->node2->child, e->weight);
    m_edge.unlock();
}

/**
 * @brief Coarsen the graph based on SHEM algorithm
 * @param originalGraph
 * @return coarsened graph
 */
Graph* coarseGraph_p(Graph* originalGraph){
    auto coarse_graph = new Graph();
    int index = 0;
    vector<bool> matchedNodes(originalGraph->V(), false);
    vector<future<void>> futures;

    auto orderedNodes = sortNodes(originalGraph->nodes);
    for(auto&n: orderedNodes){
        if(matchedNodes[n->id])
            continue;

        auto sortedEdges = sortEdge(n->edges);
        for(auto&e :sortedEdges){
            if(!matchedNodes[e->node1->id] && !matchedNodes[e->node2->id]){
                matchedNodes[e->node1->id] = matchedNodes[e->node2->id] =  true;
                futures.emplace_back(async(launch::any, operation, coarse_graph, e->node1, e->node2, index));
                index++;
                break;
            }
        }

        if(!matchedNodes[n->id]){
            m_node.lock();
            n->child = coarse_graph->add_node(index, n->weight);
            m_node.unlock();
            index++;
        }
    }

    coarse_graph->nodes.resize(index);

    for(auto& f : futures)
        f.wait();

    vector<future<void>> futures2;
    for(auto& e : originalGraph->edges){
        if(e->node1->child != e->node2->child) {
            futures2.emplace_back(async(launch::any, operation2, coarse_graph, e));
            //coarse_graph->add_or_sum_edge(e->node1->child, e->node2->child, e->weight);
        }
    }

    for(auto& f : futures2)
        f.wait();

    return coarse_graph;
}