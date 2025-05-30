#include <iostream>

// Node structure for the circular linked list
class NodeType {
public:
    char info;
    NodeType* link;
    
    NodeType(char data) {
        info = data;
        link = nullptr;
    }
};

// Circular Queue class using linked list
class CircularQueue {
private:
    NodeType* rear;        // Points to the last node (rear of queue)
    int count;             // Current number of elements
    int capacity;          // Maximum capacity of the queue

public:
    // Constructor
    CircularQueue(int size) {
        rear = nullptr;
        count = 0;
        capacity = size;
    }

    // Destructor
    ~CircularQueue() {
        // Delete all nodes in the circular queue
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Add an element to the queue
    void enqueue(char item) {
        if (isFull()) {
            std::cout << "Queue Overflow: Cannot enqueue element '" << item << "'" << std::endl;
            return;
        }
        
        NodeType* newNode = new NodeType(item);
        
        if (isEmpty()) {
            // First node - points to itself
            rear = newNode;
            rear->link = rear;
        } else {
            // Insert after rear and update rear
            newNode->link = rear->link;  // New node points to front
            rear->link = newNode;        // Old rear points to new node
            rear = newNode;              // Update rear to new node
        }
        
        count++;
        std::cout << "Enqueued: " << item << std::endl;
    }

    // Remove an element from the queue
    char dequeue() {
        if (isEmpty()) {
            std::cout << "Queue Underflow: Cannot dequeue from an empty queue" << std::endl;
            return '\0';
        }
        
        NodeType* front = rear->link;  // Front is next to rear
        char item = front->info;
        
        if (count == 1) {
            // Only one node - queue becomes empty
            delete front;
            rear = nullptr;
        } else {
            // Update rear to point to new front
            rear->link = front->link;
            delete front;
        }
        
        count--;
        return item;
    }

    // Get the front element without removing it
    char peek() {
        if (isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            return '\0';
        }
        return rear->link->info;  // Front is next to rear
    }

    // Check if queue is empty
    bool isEmpty() {
        return (rear == nullptr);
    }

    // Check if queue is full
    bool isFull() {
        return (count == capacity);
    }

    // Get the current size of the queue
    int size() {
        return count;
    }

    // Display all elements in the queue
    void display() {
        if (isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            return;
        }
        
        std::cout << "Circular Queue elements: ";
        NodeType* current = rear->link;  // Start from front
        
        for (int i = 0; i < count; i++) {
            std::cout << current->info << " ";
            current = current->link;
        }
        std::cout << std::endl;
    }
};

int main() {
    // Create a circular queue with capacity 5
    CircularQueue queue(5);
    
    // Enqueue operations
    queue.enqueue('A');
    queue.enqueue('B');
    queue.enqueue('C');
    queue.enqueue('D');
    queue.enqueue('E');
    
    // Queue is full now, this should show overflow message
    queue.enqueue('F');
    
    // Display the queue
    queue.display();
    
    // Dequeue operations
    std::cout << "Dequeued: " << queue.dequeue() << std::endl;
    std::cout << "Dequeued: " << queue.dequeue() << std::endl;
    
    // Display after dequeue
    queue.display();
    
    // Now we can enqueue more items as space has been freed
    queue.enqueue('G');
    queue.enqueue('H');
    
    // Display the queue
    queue.display();
    
    // Show front element without removing
    std::cout << "Front element: " << queue.peek() << std::endl;
    
    // Display queue size
    std::cout << "Queue size: " << queue.size() << std::endl;
    
    return 0;
}