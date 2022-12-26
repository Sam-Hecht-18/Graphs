//node.h
//Author: Sam Hecht
//Purpose: Struct declarations for a node of the graph/priority queue
//Date: 12/18/22

#ifndef NODE_H
#define NODE_H
#include <list>

struct Node;

struct Edge {
    Node *source;
    Node *dest;
    int weight;

    Edge(Node *source, Node *dest, int weight) {
        this->source = source;
        this->dest = dest;
        this->weight = weight;
    }

};

struct Node {
    int id;
    std::list <Edge> adj;
    int dist;
    int index;
    Node *prev;
};

#endif
