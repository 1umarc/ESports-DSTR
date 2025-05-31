#pragma once
#include <iostream>

// Node structure for the linked list
class NodeTypeQueue {
public:
    char info;
    NodeTypeQueue* link;
};

// Queue class using linked list
class Queue {
private:
    NodeTypeQueue* queueFront;  // queue's first node (head)
    NodeTypeQueue* queueRear;   // queue's last node (tail)
    int count;             // to keep track of number of elements in the queue

public:
    // Constructor
    Queue() {
        queueFront = nullptr;
        queueRear = nullptr;
        count = 0;
    }

    // Destructor
    ~Queue() {
        // Delete all nodes in the queue
        while (!isEmpty()) {
            deleteQueue();
        }
    }

    // Add element to the end of the queue
    void addQueue(char elem) {
        // Create a new node
        NodeTypeQueue* newNode = new NodeTypeQueue;
        newNode->info = elem;
        newNode->link = nullptr;

        // If queue is empty, front and rear point to the new node
        if (isEmpty()) {
            queueFront = newNode;
            queueRear = newNode;
        } else {
            // Otherwise, add node to the end and update rear
            queueRear->link = newNode;
            queueRear = newNode;
        }

        // Increment count
        count++;
    }

    // Delete the first node and return its info
    char deleteQueue() {
        if (isEmpty()) {
            std::cout << "Queue is empty. Cannot delete element." << std::endl;
            return '\0';  // Return null character for empty queue
        }

        // Save front node and its info
        NodeTypeQueue* tempNode = queueFront;
        char tempInfo = queueFront->info;

        // Update front to the next node
        queueFront = queueFront->link;

        // If queue becomes empty, update rear to nullptr as well
        if (queueFront == nullptr) {
            queueRear = nullptr;
        }

        // Delete the old front node and decrement count
        delete tempNode;
        count--;

        return tempInfo;
    }

    // Check if queue is empty
    bool isEmpty() {
        return (queueFront == nullptr);
    }

    // Return number of elements in the queue
    int size() {
        return count;
    }

    // Display all elements in the queue
    void display() {
        if (isEmpty()) {
            std::cout << "Queue is empty." << std::endl;
            return;
        }

        std::cout << "Queue elements: ";
        NodeTypeQueue* current = queueFront;
        while (current != nullptr) {
            std::cout << current->info << " ";
            current = current->link;
        }
        std::cout << std::endl;
    }
};
