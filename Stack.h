#pragma once
#include <iostream>
using namespace std;

class StackNode {
public:
    char data;
    StackNode* next;
    
    StackNode(char new_data) {
        this->data = new_data;
        this->next = nullptr;
    }
};

class Stack {
private:
    StackNode* head;
    int count;

public:
    Stack() { 
        this->head = nullptr; 
        this->count = 0;
    }
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool isEmpty() {
        return head == nullptr;
    }
    
    int size() {
        return count;
    }

    void push(char new_data) {
        StackNode* new_node = new StackNode(new_data);
        if (!new_node) {
            cout << "\nStack Overflow";
            return;
        }
        new_node->next = head;
        head = new_node;
        count++;
    }

    void pop() {
        if (this->isEmpty()) {
            cout << "\nStack Underflow" << endl;
        } else {
            StackNode* temp = head;
            head = head->next;
            delete temp;
            count--;
        }
    }

    char peek() {
        if (!isEmpty())
            return head->data;
        else {
            cout << "\nStack is empty";
            return '\0';
        }
    }
    
    void display() {
        if (isEmpty()) {
            cout << "Stack is empty" << endl;
            return;
        }
        
        cout << "Stack contents (top to bottom): ";
        StackNode* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};