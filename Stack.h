#pragma once
#include <iostream>
#include <string>
using namespace std;

// Node structure for the stack
class SNode 
{
public:
    string data;
    SNode* next;
    
    // Constructor to initialize the node
    SNode(const string& new_data) 
    {
        this->data = new_data;
        this->next = nullptr;
    }
};

class Stack 
{
private:
    SNode* head;  // Head of the stack
    int count;    // Number of items in the stack

public:
    // Constructor to initialize the stack
    Stack() 
    { 
        this->head = nullptr; 
        this->count = 0;
    }
    
    // Destructor to deallocate memory
    ~Stack() 
    {
        while (!isEmpty()) 
        {
            pop();
        }
    }

    // Check if the stack is empty
    bool isEmpty() 
    {
        return head == nullptr;
    }
    
    // Return the number of items in the stack
    int size() 
    {
        return count;
    }

    // Add an item to the stack
    void push(const string& new_data) 
    {
        SNode* new_node = new SNode(new_data);
        if (!new_node) 
        {
            cout << "\nStack Overflow";
            return;
        }
        new_node->next = head;
        head = new_node;
        count++;
    }

    // Remove an item from the stack
    void pop() 
    {
        if (this->isEmpty()) 
        {
            cout << "\nStack Underflow" << endl;
        } 
        else 
        {
            SNode* temp = head;
            head = head->next;
            delete temp;
            count--;
        }
    }

    // Return the top item of the stack
    string peek() 
    {
        if (!isEmpty())
            return head->data;
        else 
        {
            cout << "\nStack is empty";
            return "";
        }
    }
    
    // Display the stack contents
    void display() 
    {
        if (isEmpty()) 
        {
            cout << "Stack is empty" << endl;
            return;
        }
        
        cout << "Stack contents (top to bottom): ";
        SNode* current = head;
        while (current != nullptr) 
        {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};
