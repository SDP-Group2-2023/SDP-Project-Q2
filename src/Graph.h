#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>
#include <set>
#include <barrier>
#include <mutex>

using namespace std;

struct Node{
    int id;
    int weight;
};

struct Edge{
    int source;
    int dest;
    int weight;

    bool operator<(const Edge &a) const {
        return this->weight > a.weight;
    }

    bool operator>(const Edge &a) const {
        return this->weight < a.weight;
    }

    bool operator==(const Edge &c) const {
        return this->source == c.source && this->dest == c.dest;
    }
};

class Graph{
    private:
        set<Edge> edges;
        vector<Node> nodes;
        mutex mNode;
        mutex mEdge;
        void thread_reader(const string &path, unsigned long offset_from_start_nodes,
                                  int nodes_to_read, unsigned long offset_from_start_edges, 
                                  int edges_to_read);
    public:
        Graph(const string& path);
        void add_node(int id, int weight);
        void add_edge(int source, int dest, int weight);
        Node get_node(int id);
        Edge get_edge(int source, int dest);
        Edge get_next_max_edge(vector<bool> &matched, vector<int> &partitions);
        int get_num_nodes();
        int get_num_edges();
};

#endif
