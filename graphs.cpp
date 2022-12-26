//graphs.cpp
//Author: Sam Hecht
//Date: 12/18/22
//Purpose: To implement some of the graph algorithms that I learned about
// during my Algorithms class
#include <iostream>
#include <limits.h>
#include <list>
#include <queue>
#include "node.h"
#include "priorityQueue.h"
using namespace std;


void dijkstra(Node **nodes, int numNodes, int sourceNode);
bool bellmanFord(Node **nodes, int numNodes, int sourceNode);
void relax(PriorityQueue &pq, Edge &edge);
void relax(bool &changed, Edge &edge);
void printGraph(Node **nodes, int numNodes, bool isMST, int sourceNode = 0);
void prims(Node **nodes, int numNodes, int sourceNode = 0);


int main(int argc, char *argv[]) {
    bool directed = false;
    bool isMST = false;
    if (argc != 3) {
        cerr << "USAGE: ./EXECUTABLE [dir/undir] [MST/SSSP]" << endl;
        exit(0);
    }
    else {
        string argString = argv[1];
        directed = argString == "dir";
        argString = argv[2];
        isMST = argString == "MST";
    }
    //Display whether the user correctly 
    cerr << "Directed was: " << (directed ? "true" : "false") << endl;
    cerr << "MST was: " << (isMST ? "true" : "false") << endl;
    cout << "Number of nodes: ";
    int numNodes, numEdges, sourceNode;
    cin >> numNodes;
    cout << "Number of edges: ";
    cin >> numEdges;
    if (not isMST) {
        cout << "Index of source node: ";
        cin >> sourceNode;
    }

    Node **nodes = new Node*[numNodes];
    //Build nodes
    for (int i = 0; i < numNodes; i++) {
        nodes[i] = new Node;
        nodes[i]->id = i;
    }
    //Add edges
    int source, dest, weight;
    bool negativeWeight = false;
    for (int i = 0; i < numEdges; i++) {
        cin >> source >> dest >> weight;
        if (weight < 0) {
            negativeWeight = true;
        }
        nodes[source]->adj.push_back(Edge(nodes[source], nodes[dest], 
                                                            weight));
        //Adds 'backwards' edges if the graph is undirected                                                    
        if (not directed) {
            nodes[dest]->adj.push_back(Edge(nodes[dest], nodes[source], 
                                                            weight));
        }                                                
        
    }
    if (not isMST) {
        //Run dijkstra's algorithm to find the shortest path if there are no
        //nodes with negative weights
        if (not negativeWeight) {
            dijkstra(nodes, numNodes, sourceNode);
            printGraph(nodes, numNodes, isMST, sourceNode);
        }
        //Run Bellman-Ford's algorithm to find shortest path with neg. weights
        else {
            if(bellmanFord(nodes, numNodes, sourceNode)) {
                printGraph(nodes, numNodes, isMST, sourceNode);
            }
            else {
                cout << "Negative cycle detected!" << endl;
            }
        }
    }
    else {
        //Run prims to create an MST of the graph
        prims(nodes, numNodes);
        printGraph(nodes, numNodes, isMST);
    }
    

        
    return 0;
}

//printGraph
//Purpose: To print the result of running dijkstra's bellman-ford's or prim's
//         on the graph
//Parameters: Node **nodes - An array of pointers to nodes, representing the 
//                           graph
//            int numNodes - The number of nodes in the graph
//            bool isMST - A bool representing whether the graph is an MST
//            int sourceNode - The index of the source node of the graph 
//Return: N/A (void)
void printGraph(Node **nodes, int numNodes, bool isMST, int sourceNode) {
    string dist_message = "The distance of node: ";
    string origin_message = " from the source is: ";
    if (isMST) {
        dist_message = "The edge preceding node: ";
        origin_message = " has a weight of: ";
    }
    int totalWeight = 0;
    cout << "Source node is: " << sourceNode << endl << endl;
    //Prints each node's score/dist and its previous node
    for (int i = 0; i < numNodes; i++) {
        cout << dist_message << i << origin_message << nodes[i]->dist << endl;
        totalWeight += nodes[i]->dist;
        if (nodes[i]->prev != nullptr) {
            cout << "The previous node was: " << nodes[i]->prev->id << endl;
        }
        else {
            cout << "This node has no previous node" << endl;
        }
    }
    if (isMST) {
        cout << "The total weight of the MST is: " << totalWeight << endl;
    }
}

//dijkstra
//Purpose: To perform dijkstra's algorithm on the parameterized graph nodes,
//         updating each node with a dist and a previous node as necessary
//Parameters: Node **nodes - An array of pointers to nodes, representing the 
//                           graph
//            int numNodes - The number of nodes in the graph
//            int sourceNode - The index of the source node of the graph
//Return: N/A (void)
void dijkstra(Node **nodes, int numNodes, int sourceNode) {
    PriorityQueue pq(numNodes, sourceNode, nodes);
    while (not pq.empty()) {
        Node *reached = pq.extractMin();
        for (list<Edge>::iterator it = reached->adj.begin(); 
                                            it != reached->adj.end(); it++) {

            
            relax(pq, *it);
        }
    }  
}

//relax
//Purpose: To relax (i.e. lower the score/dist) nodes for dijkstra's algorithm 
//         if necessary
//Parameters: PriorityQueue &pq - The priority queue of dijkstra's algorithm
//            Edge &edge - The edge to attempt to relax the destination node's
//                         weight of
//Return: N/A (void)
void relax(PriorityQueue &pq, Edge &edge) {
    if (pq.contains(edge.dest) and  
        edge.dest->dist > edge.source->dist + edge.weight) {
        edge.dest->dist = edge.source->dist + edge.weight;
        pq.reduceKey(edge.dest);
        edge.dest->prev = edge.source;
    }
}

//relax
//Purpose: To relax (i.e. lower the score/dist) nodes for bellman-ford's 
//         algorithm if necessary
//Parameters: bool &changed - The bool representing whether any node's were 
//                            relaxed
//            Edge &edge - The edge to attempt to relax the destination node's
//                         weight of
//Return: N/A (void)
void relax(bool &changed, Edge &edge) {
    if (edge.source->dist != INT_MAX and 
        edge.dest->dist > edge.source->dist + edge.weight) {
        edge.dest->dist = edge.source->dist + edge.weight;
        changed = true;
        edge.dest->prev = edge.source;
    }
}


//bellmanFord
//Purpose: To perform bellman-ford's algorithm on the parameterized graph nodes,
//         updating each node with a dist and a previous node as necessary
//Parameters: Node **nodes - An array of pointers to nodes, representing the 
//                           graph
//            int numNodes - The number of nodes in the graph
//            int sourceNode - The index of the source node of the graph
//Return: A bool representing whether a negative weight cycle was detected
bool bellmanFord(Node **nodes, int numNodes, int sourceNode) {
    list<Edge> edges;
    //Create the list of edges which we'll iterate over at most numNodes-1 times
    for (int i = 0; i < numNodes; i++) {
        for (list<Edge>::iterator it = nodes[i]->adj.begin(); 
                it != nodes[i]->adj.end(); it++) {

            edges.push_back(*it);
        }
        nodes[i]->dist = INT_MAX;
    }
    nodes[sourceNode]->dist = 0;
    bool changed = true;
    //Iterate through all of the edges at most numNodes-1 times, relaxing each
    //edge each time
    for (int i = 0; i < numNodes - 1 and changed; i++) {
        changed = false;
        for (list<Edge>::iterator it = edges.begin(); it != edges.end(); it++) {
            relax(changed, *it);
        }
    }
    changed = false;
    //Check for negative weight cycles
    for (list<Edge>::iterator it = edges.begin(); it != edges.end(); it++) {
        relax(changed, *it);
    }
    return not changed;
}


//prims
//Purpose: To generate an MST of the graph given by Node **nodes using prim's
//         algorithm
//Parameters: Node **nodes - An array of pointers to nodes, representing the 
//                           graph
//            int numNodes - The number of nodes in the graph
//            int sourceNode - defaults to 0, the index of the source node for
//                             the MST
void prims(Node **nodes, int numNodes, int sourceNode) {
    PriorityQueue pq(numNodes, sourceNode, nodes);
    while (not pq.empty()) {
        Node *reached = pq.extractMin();
        //For each edge of the node we just popped, see if we can decrease
        //the score of any of the destination nodes
        for (list<Edge>::iterator it = reached->adj.begin();
             it != reached->adj.end(); it++) {

            if (pq.contains(it->dest) and it->dest->dist > it->weight) {
                it->dest->dist = it->weight;
                it->dest->prev = it->source;
                pq.reduceKey(it->dest);
            }
        }
    }
}

