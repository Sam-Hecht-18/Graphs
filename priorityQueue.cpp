//priorityQueue.cpp
//Author: Sam Hecht
//Purpose: To implement a PriorityQueue using a min heap
//Date: 12/18/22

#include "priorityQueue.h"
#include <iostream>
#include <limits.h>

//Creates and builds the priority queue with the sourceNode as the root
PriorityQueue::PriorityQueue(int numNodes, int sourceNode, Node **nodes) {
    size = numNodes;
    priorityQueue = new Node*[size + 1];
    int subtract = 0;
    priorityQueue[0] = nullptr;
    //Builds the priorityQueue with each node having a dist of infinity except
    //the source, which has a dist of 0, and is at index 1. Indices of all other
    //nodes are effectively random (numerical order of id)
    for (int i = 0; i < numNodes; i++) {
        if (sourceNode == i) {
            priorityQueue[1] = nodes[sourceNode];
            nodes[sourceNode]->dist = 0;
            priorityQueue[1]->prev = nullptr;
            priorityQueue[1]->index = 1;
            subtract = 1;
        }
        else {
            priorityQueue[i + 2 - subtract] = nodes[i];
            nodes[i]->dist = INT_MAX;
            priorityQueue[i]->prev = nullptr;
            priorityQueue[i]->index = i;
        }
        
    }
    
}

//Returns whether a particular node is in the priorityQueue in O(1)
bool PriorityQueue::contains(Node *node) {
    return node->index <= size;
}

//Returns whether the priorityQueue is empty or not
bool PriorityQueue::empty() {
    return size == 0;
}

//Reduces the key by bubbling up the parameterized node in the heap
void PriorityQueue::reduceKey(Node *node) {
    while (parent(node) != nullptr and node->dist < parent(node)->dist) {
        swap(node, parent(node));
    }
}

//Returns the parent of the parameterized node, nullptr if root
//(priorityQueue[0] == nullptr)
Node *PriorityQueue::parent(Node *node) {
    return priorityQueue[node->index / 2];
}

//Returns the left child of the parameterized node, otherwise nullptr
Node *PriorityQueue::leftChild(Node *node) {
    if (node->index * 2 < size + 1) {
        return priorityQueue[node->index * 2];
    }
    else {
        return nullptr;
    }
}
//Returns the right child of the parameterized node, otherwise nullptr
Node *PriorityQueue::rightChild(Node *node) {
    if (node->index * 2 + 1 < size + 1) {
        return priorityQueue[node->index * 2 + 1];
    }
    else {
        return nullptr;
    }
}
       
//Extracts and returns the min node of the priority queue, maintaining the
//heap invariant by bubbling the new min node down
Node *PriorityQueue::extractMin() {
    Node *toReturn = priorityQueue[FIRST];
    priorityQueue[FIRST] = priorityQueue[size];
    priorityQueue[FIRST]->index = FIRST;
    toReturn->index = size;
    size--;
    bubbleDown();
    return toReturn;
}

//A print function used for debugging
void PriorityQueue::print() {
    std::cerr << "PRINTING PRIORITY QUEUE: " << std::endl;
    for (int i = 1; i < size + 1; i++) {
        std::cerr << priorityQueue[i]->id << "(" << priorityQueue[i]->dist
                  << ")" << std::endl;
    }
    std::cerr << std::endl;
}

//Bubbles down root node after putting the rightmost leaf at the root
void PriorityQueue::bubbleDown() {
    Node *toBubbleDown =  priorityQueue[FIRST];
    while ((leftChild(toBubbleDown) != nullptr and 
            (toBubbleDown->dist > leftChild(toBubbleDown)->dist)) or
           (rightChild(toBubbleDown) != nullptr and 
           toBubbleDown->dist > rightChild(toBubbleDown)->dist)) {

        Node *left = leftChild(toBubbleDown);
        Node *right = rightChild(toBubbleDown);
        if (right == nullptr or left->dist < right->dist) {
            swap(left, toBubbleDown);
        }
        else {
            swap(right, toBubbleDown);
        }  
    }
}

//Swaps the position of node1 and node2 in the priorityQueue
void PriorityQueue::swap(Node *node1, Node *node2) {
    int node2Index = node2->index;
    priorityQueue[node1->index] = node2;
    node2->index = node1->index;
    priorityQueue[node2Index] = node1;
    node1->index = node2Index;
    
}



