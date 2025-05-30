#include <iostream>
#include <string>

class LinkedListPriorityQueue {
private:
    struct Node {
        std::string data;
        int priority;
        Node* next;
        
        Node(const std::string& d, int p) : data(d), priority(p), next(nullptr) {}
    };
    
    Node* head;
    
public:
    LinkedListPriorityQueue() : head(nullptr) {}
    
    ~LinkedListPriorityQueue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void enqueue(const std::string& item, int priority) {
        Node* newNode = new Node(item, priority);
        
        // Insert at head if queue is empty or new node has highest priority
        if (!head || priority > head->priority) {
            newNode->next = head;
            head = newNode;
            return;
        }
        
        // Find the correct position to insert
        Node* current = head;
        while (current->next && current->next->priority >= priority) {
            current = current->next;
        }
        
        newNode->next = current->next;
        current->next = newNode;
    }
    
    std::string dequeue() {
        if (!head) return "Queue is empty!";
        
        std::string result = head->data;
        Node* temp = head;
        head = head->next;
        delete temp;
        
        return result;
    }
    
    std::string peek() {
        if (!head) return "Queue is empty!";
        return head->data;
    }
    
    bool isEmpty() { return head == nullptr; }
};

int main() {
    LinkedListPriorityQueue pq;
    pq.enqueue("Task 1", 3);
    pq.enqueue("Task 2", 1);
    pq.enqueue("Task 3", 2);
    
    std::cout << "Dequeued: " << pq.dequeue() << "\n";  // Task 1
    std::cout << "Peek: " << pq.peek() << "\n";        // Task 3
    std::cout << "Dequeued: " << pq.dequeue() << "\n";  // Task 3
    
    return 0;
}