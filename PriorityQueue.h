#pragma once
#include <iostream>
#include <string>
using namespace std;

class PQNode 
{
public:
    string data;
    int priority;
    PQNode* next;
    
    PQNode(const string& d, int p) : data(d), priority(p), next(nullptr) {}
};

class PriorityQueue 
{
private:
    PQNode* head;
    int count;
    
public:
    PriorityQueue() : head(nullptr), count(0) {}
    
    ~PriorityQueue() 
    {
        while (head) 
        {
            PQNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void enqueue(const string& item, int priority) 
    {
        PQNode* newNode = new PQNode(item, priority);
        
        // Insert at head if queue is empty or new node has highest priority
        // Higher priority value = higher priority (1 > 2 > 3)
        if (!head || priority < head->priority) 
        {
            newNode->next = head;
            head = newNode;
            count++;
            return;
        }
        
        // Find the correct position to insert
        PQNode* current = head;
        while (current->next && current->next->priority <= priority) 
        {
            current = current->next;
        }
        
        newNode->next = current->next;
        current->next = newNode;
        count++;
    }
    
    string dequeue() 
    {
        if (!head) return "";
        
        string result = head->data;
        PQNode* temp = head;
        head = head->next;
        delete temp;
        count--;
        
        return result;
    }
    
    string peek() 
    {
        if (!head) return "";
        return head->data;
    }
    
    bool isEmpty() 
    { 
        return head == nullptr; 
    }
    
    int size() 
    {
        return count;
    }
    
    void display() 
    {
        if (isEmpty()) 
        {
            cout << "[CONSOLE LOG] Priority Queue is empty" << endl;
            return;
        }
        
        cout << "[CONSOLE LOG] Priority Queue contents (highest to lowest priority): ";
        PQNode* current = head;
        while (current != nullptr) 
        {
            cout << "[" << current->data << " (P:" << current->priority << ")] ";
            current = current->next;
        }
        cout << endl;
    }
};