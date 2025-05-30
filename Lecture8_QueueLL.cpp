#include <iostream>

// Node structure for the linked list
class NodeType {
public:
    char info;
    NodeType* link;
};

// Queue class using linked list
class Queue {
private:
    NodeType* queueFront;  // queue's first node (head)
    NodeType* queueRear;   // queue's last node (tail)
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
        NodeType* newNode = new NodeType;
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
        NodeType* tempNode = queueFront;
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
        NodeType* current = queueFront;
        while (current != nullptr) {
            std::cout << current->info << " ";
            current = current->link;
        }
        std::cout << std::endl;
    }
};

// Example usage
int main() {
    Queue charQueue;
    
    // Add elements to the queue
    charQueue.addQueue('A');
    charQueue.addQueue('B');
    charQueue.addQueue('C');
    
    // Display queue and its size
    charQueue.display();
    std::cout << "Queue size: " << charQueue.size() << std::endl;
    
    // Remove elements
    std::cout << "Removed: " << charQueue.deleteQueue() << std::endl;
    
    // Display queue again
    charQueue.display();
    std::cout << "Queue size: " << charQueue.size() << std::endl;
    
    // Add more elements
    charQueue.addQueue('D');
    charQueue.addQueue('E');
    
    // Display queue
    charQueue.display();
    std::cout << "Queue size: " << charQueue.size() << std::endl;
    
    // Empty the queue
    while (!charQueue.isEmpty()) {
        std::cout << "Removed: " << charQueue.deleteQueue() << std::endl;
    }
    
    // Check if queue is empty
    std::cout << "Is queue empty? " << (charQueue.isEmpty() ? "Yes" : "No") << std::endl;
    
    return 0;
}