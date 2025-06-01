#pragma once
#include <iostream>
#include <string>
using namespace std;

// Node structure for the circular linked list
class CQNode 
{
public:
    string info;
    CQNode* link;
    
    CQNode(const string& data) 
    {
        info = data;
        link = nullptr;
    }
};

// Circular Queue class using linked list
class CircularQueue 
{
private:
    CQNode* rear;          // Points to the last node (rear of queue)
    int count;             // Current number of elements
    int capacity;          // Maximum capacity of the queue

public:
    // Constructor
    CircularQueue(int size) 
    {
        rear = nullptr;
        count = 0;
        capacity = size;
    }

    // Destructor
    ~CircularQueue() 
    {
        // Delete all nodes in the circular queue
        while (!isEmpty()) 
        {
            dequeue();
        }
    }

    // Add an element to the queue
    void enqueue(const string& item) 
    {
        if (isFull())
        {
            cout << "Queue Overflow: Cannot enqueue element '" << item << "'" << endl;
            return;
        }
        
        CQNode* newNode = new CQNode(item);
        
        if (isEmpty()) 
        {
            // First node - points to itself
            rear = newNode;
            rear->link = rear;
        } 
        else 
        {
            // Insert after rear and update rear
            newNode->link = rear->link;  // New node points to front
            rear->link = newNode;        // Old rear points to new node
            rear = newNode;              // Update rear to new node
        }
        
        count++;
        cout << "Enqueued: " << item << endl;
    }

    // Remove an element from the queue
    string dequeue() 
    {
        if (isEmpty()) 
        {
            cout << "Queue Underflow: Cannot dequeue from an empty queue" << endl;
            return "";
        }
        
        CQNode* front = rear->link;  // Front is next to rear
        string item = front->info;
        
        if (count == 1) 
        {
            // Only one node - queue becomes empty
            delete front;
            rear = nullptr;
        } 
        else 
        {
            // Update rear to point to new front
            rear->link = front->link;
            delete front;
        }
        
        count--;
        return item;
    }

    // Get the front element without removing it
    string peek() 
    {
        if (isEmpty()) 
        {
            cout << "Queue is empty" << endl;
            return "";
        }
        return rear->link->info;  // Front is next to rear
    }

    // Check if queue is empty
    bool isEmpty() 
    {
        return (rear == nullptr);
    }

    // Check if queue is full
    bool isFull() 
    {
        return (count == capacity);
    }

    // Get the current size of the queue
    int size() 
    {
        return count;
    }

    // Display all elements in the queue
    void display() 
    {
        if (isEmpty()) 
        {
            cout << "Queue is empty" << endl;
            return;
        }
        
        cout << "Circular Queue elements: ";
        CQNode* current = rear->link;  // Start from front
        
        for (int i = 0; i < count; i++) 
        {
            cout << current->info << " ";
            current = current->link;
        }
        cout << endl;
    }
};