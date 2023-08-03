#include "partitioning.h"

vector<Node*> sortNodes(const vector<Node*>& nodes) {
    vector<Node *> sortedNodes = nodes;
    sort(sortedNodes.begin(), sortedNodes.end(), [](Node *n1, Node *n2) {
        return n1->weight < n2->weight;
    });
    return sortedNodes;
}

Graph* coarseGraph(Graph* originalGraph, int num_partitions){

    auto coarse_graph = new Graph();
    int index = 0;
    vector<bool> matchedNodes(originalGraph->V, false);

    auto orderedNodes = sortNodes(originalGraph->nodes);
    for(auto&n: orderedNodes){
        bool flag = false;

        auto sortedEdges = n->edges;
        sort(sortedEdges.begin(), sortedEdges.end(), [](auto e1, auto e2) {
            return e1->weight > e2->weight;
        });

        for(auto&e :sortedEdges){
            if(!matchedNodes[e->node1->id] && !matchedNodes[e->node2->id]){
                Node*n1 = e->node1;
                Node*n2 = e->node2;
                Node*newNode = coarse_graph->add_node(index, n1->weight + n2->weight);
                newNode->parent1 = n1;
                newNode->parent2 = n2;
                n1->child = newNode;
                n2->child = newNode;

                matchedNodes[n1->id] = true;
                matchedNodes[n2->id] = true;
                flag = true;
                index++;
            }
        }
        if(!flag && !matchedNodes[n->id]){
            Node*newNode = coarse_graph->add_node(index, n->weight);
            newNode->parent1 = n;
            n->child = newNode;
            index++;
        }
    }

    for(auto&n : originalGraph->nodes){
        for(auto&e : n->edges){
            if(e->node1->child != e->node2->child)
                coarse_graph->add_or_sum_edge(e->node1->child->id,e->node2->child->id, e->weight);
        }
    }

    return coarse_graph;
}