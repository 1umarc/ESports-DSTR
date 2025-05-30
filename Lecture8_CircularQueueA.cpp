#include <iostream>

class CircularQueue {
private:
    char* array;
    int capacity;      // Maximum capacity of the queue
    int front;         // Index of the front element
    int rear;          // Index of the rear element
    int count;         // Current number of elements

public:
    // Constructor
    CircularQueue(int size) {
        capacity = size;
        array = new char[capacity];
        front = 0;
        rear = -1;
        count = 0;
    }

    // Destructor
    ~CircularQueue() {
        delete[] array;
    }

    // Add an element to the queue
    void enqueue(char item) {
        if (isFull()) {
            std::cout << "Queue Overflow: Cannot enqueue element '" << item << "'" << std::endl;
            return;
        }
        
        // Update rear in circular fashion
        rear = (rear + 1) % capacity;
        array[rear] = item;
        count++;
        
        std::cout << "Enqueued: " << item << std::endl;
    }

    // Remove an element from the queue
    char dequeue() {
        if (isEmpty()) {
            std::cout << "Queue Underflow: Cannot dequeue from an empty queue" << std::endl;
            return '\0';
        }
        
        char item = array[front];
        front = (front + 1) % capacity;
        count--;
        
        return item;
    }

    // Get the front element without removing it
    char peek() {
        if (isEmpty()) {
            std::cout << "Queue is empty" << std::endl;
            return '\0';
        }
        return array[front];
    }

    // Check if queue is empty
    bool isEmpty() {
        return (count == 0);
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
        int current = front;
        for (int i = 0; i < count; i++) {
            std::cout << array[current] << " ";
            current = (current + 1) % capacity;
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