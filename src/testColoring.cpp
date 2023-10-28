#include "Graph.h"
#include "partitioning.h"

int main(){


    std::shared_ptr<Graph> g( new Graph());
    g->add_node(0, 5);
    g->add_node(1, 5);
    g->add_node(2, 5);
    g->add_node(3, 5);
    g->add_node(4, 5);
    g->add_node(5, 5);
    g->add_node(6, 5);
    g->add_node(7, 5);
    g->add_node(8, 5);
    g->add_node(9, 5);
    g->add_node(10, 5);

    //node 0
    g->add_edge(0, 1, 1);
    g->add_edge(0, 2, 1);
    g->add_edge(0, 3, 1);

    //node 1
    g->add_edge(1, 2, 1);
    g->add_edge(1, 4, 1);

    //node 2
    g->add_edge(2, 3, 1);
    g->add_edge(2, 4, 1);
    g->add_edge(2, 6, 1);

    //node 3
    g->add_edge(3, 7, 1);

    //node 4
    g->add_edge(4, 5, 1);
    g->add_edge(4, 6, 1);

    //node 5
    g->add_edge(5, 6, 1);
    g->add_edge(5, 8, 1);

    //node 6
    g->add_edge(6, 7, 1);
    g->add_edge(6, 8, 1);
    g->add_edge(6, 9, 1);

    //node 7
    g->add_edge(7, 9, 1);

    //node 8
    g->add_edge(8, 9, 1);

    //node 9
    g->add_edge(9, 10, 1);


    auto newG = coarseGraph_p(g, 2);



}