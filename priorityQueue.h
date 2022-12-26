//priorityQueue.h
//Author: Sam Hecht
//Purpose: Header file for a PriorityQueue using a min heap
//Date: 12/18/22
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "node.h"

class PriorityQueue {
    public:
        PriorityQueue(int numNodes, int sourceNode, Node **nodes);
        void reduceKey(Node *node);
        Node *extractMin();
        bool empty();

        
        bool contains(Node *node);

        
    private:
        static const int FIRST = 1;
        int size;
        Node **priorityQueue;
        void print();
        void bubbleDown();
        Node *parent(Node *node);
        Node *leftChild(Node *node);
        Node *rightChild(Node *node);
        void swap(Node *node1, Node *node2);
};

#endif
