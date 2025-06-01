#pragma once
#include <iostream>
#include <string>
using namespace std;

// Node structure for the linked list
class QNode 
{
public:
    string info;
    QNode* link;
};

// Queue class using linked list
class Queue 
{
private:
    QNode* queueFront;  // queue's first node (head)
    QNode* queueRear;   // queue's last node (tail)
    int count;          // to keep track of number of elements in the queue

public:
    // Constructor
    Queue() 
    {
        queueFront = nullptr;
        queueRear = nullptr;
        count = 0;
    }

    // Destructor
    ~Queue() 
    {
        // Delete all nodes in the queue
        while (!isEmpty()) 
        {
            dequeue();
        }
    }

    // Add element to the end of the queue
    void enqueue(const string& elem)
    {
        // Create a new node
        QNode* newNode = new QNode;
        newNode->info = elem;
        newNode->link = nullptr;

        // If queue is empty, front and rear point to the new node
        if (isEmpty()) 
        {
            queueFront = newNode;
            queueRear = newNode;
        } 
        else 
        {
            // Otherwise, add node to the end and update rear
            queueRear->link = newNode;
            queueRear = newNode;
        }

        // Increment count
        count++;
    }

    // Delete the first node and return its info
    string dequeue() 
    {
        if (isEmpty()) 
        {
            cout << "Queue is empty. Cannot delete element." << endl;
            return "";  // Return empty string for empty queue
        }

        // Save front node and its info
        QNode* tempNode = queueFront;
        string tempInfo = queueFront->info;

        // Update front to the next node
        queueFront = queueFront->link;

        // If queue becomes empty, update rear to nullptr as well
        if (queueFront == nullptr)
        {
            queueRear = nullptr;
        }

        // Delete the old front node and decrement count
        delete tempNode;
        count--;

        return tempInfo;
    }

    // Check if queue is empty
    bool isEmpty() 
    {
        return (queueFront == nullptr);
    }

    // Return number of elements in the queue
    int size() 
    {
        return count;
    }

    // Display all elements in the queue
    void display() 
    {
        if (isEmpty()) 
        {
            cout << "[CONSOLE LOG] Queue is empty." << endl;
            return;
        }

        cout << "[CONSOLE LOG] Queue elements: ";
        QNode* current = queueFront;
        while (current != nullptr)
        {
            cout << "[" << current->info << "] ";
            current = current->link;
        }
        cout << endl;
    }
};