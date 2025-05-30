#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// ============================================================================
// TEMPLATE QUEUE IMPLEMENTATIONS
// ============================================================================

// Node structure for queues
template <typename T>
class QueueNode {
public:
    T data;
    QueueNode* next;
    
    QueueNode(const T& item) : data(item), next(nullptr) {}
};

// Regular Queue Template Implementation
template <typename T>
class Queue {
private:
    QueueNode<T>* front;
    QueueNode<T>* rear;
    int count;

public:
    Queue() : front(nullptr), rear(nullptr), count(0) {}
    
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    void enqueue(const T& item) {
        QueueNode<T>* newNode = new QueueNode<T>(item);
        
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        count++;
    }
    
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        
        QueueNode<T>* temp = front;
        T item = front->data;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        count--;
        return item;
    }
    
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return front->data;
    }
    
    bool isEmpty() const {
        return front == nullptr;
    }
    
    int size() const {
        return count;
    }
    
    void displayAll() const {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }
        
        QueueNode<T>* current = front;
        int position = 1;
        while (current != nullptr) {
            cout << "Position " << position << ": ";
            current->data.displayInfo();
            current = current->next;
            position++;
        }
    }
};

// Priority Queue Node with priority
template <typename T>
class PriorityNode {
public:
    T data;
    int priority;
    PriorityNode* next;
    
    PriorityNode(const T& item, int prio) : data(item), priority(prio), next(nullptr) {}
};

// Priority Queue Template Implementation
template <typename T>
class PriorityQueue {
private:
    PriorityNode<T>* head;
    int count;

public:
    PriorityQueue() : head(nullptr), count(0) {}
    
    ~PriorityQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    void enqueue(const T& item) {
        int priority = item.priority; // Assumes T has a priority member
        PriorityNode<T>* newNode = new PriorityNode<T>(item, priority);
        
        // Insert at head if queue is empty or new node has higher priority
        if (!head || priority > head->priority) {
            newNode->next = head;
            head = newNode;
        } else {
            // Find correct position to insert (maintain priority order)
            PriorityNode<T>* current = head;
            while (current->next && current->next->priority >= priority) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        count++;
    }
    
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Priority queue is empty");
        }
        
        PriorityNode<T>* temp = head;
        T item = head->data;
        head = head->next;
        delete temp;
        count--;
        return item;
    }
    
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Priority queue is empty");
        }
        return head->data;
    }
    
    bool isEmpty() const {
        return head == nullptr;
    }
    
    int size() const {
        return count;
    }
    
    void displayAll() const {
        if (isEmpty()) {
            cout << "Priority queue is empty." << endl;
            return;
        }
        
        PriorityNode<T>* current = head;
        int position = 1;
        while (current != nullptr) {
            cout << "Priority " << current->priority << " - Position " << position << ": ";
            current->data.displayInfo();
            current = current->next;
            position++;
        }
    }
};

// Circular Queue Template Implementation
template <typename T>
class CircularQueue {
private:
    QueueNode<T>* rear;
    int count;
    int capacity;

public:
    CircularQueue(int maxSize = 50) : rear(nullptr), count(0), capacity(maxSize) {}
    
    ~CircularQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    void enqueue(const T& item) {
        if (isFull()) {
            cout << "Circular queue overflow: Cannot add more items" << endl;
            return;
        }
        
        QueueNode<T>* newNode = new QueueNode<T>(item);
        
        if (isEmpty()) {
            rear = newNode;
            rear->next = rear; // Points to itself
        } else {
            newNode->next = rear->next; // New node points to front
            rear->next = newNode;       // Old rear points to new node
            rear = newNode;             // Update rear to new node
        }
        count++;
    }
    
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Circular queue is empty");
        }
        
        QueueNode<T>* front = rear->next; // Front is next to rear
        T item = front->data;
        
        if (count == 1) {
            // Only one node - queue becomes empty
            delete front;
            rear = nullptr;
        } else {
            // Update rear to point to new front
            rear->next = front->next;
            delete front;
        }
        count--;
        return item;
    }
    
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Circular queue is empty");
        }
        return rear->next->data; // Front is next to rear
    }
    
    bool isEmpty() const {
        return rear == nullptr;
    }
    
    bool isFull() const {
        return count == capacity;
    }
    
    int size() const {
        return count;
    }
    
    void displayAll() const {
        if (isEmpty()) {
            cout << "Circular queue is empty." << endl;
            return;
        }
        
        cout << "Circular queue contents (FIFO order):" << endl;
        QueueNode<T>* current = rear->next; // Start from front
        
        for (int i = 0; i < count; i++) {
            cout << "Position " << (i + 1) << ": ";
            current->data.displayInfo();
            current = current->next;
        }
    }
};

// ============================================================================
// SPECTATOR AND STREAMING MANAGEMENT CLASSES
// ============================================================================

// Spectator class to represent each spectator
class Spectator {
public:
    string name;
    string spectatorId;
    string spectatorType; // "VIP", "Influencer", "General"
    int priority;
    string arrivalTime;
    bool isSeated;
    
    Spectator() : name(""), spectatorId(""), spectatorType("General"), priority(1), 
                  arrivalTime(""), isSeated(false) {}
    
    Spectator(const string& n, const string& id, const string& type, const string& time) 
        : name(n), spectatorId(id), spectatorType(type), arrivalTime(time), isSeated(false) {
        // Assign priority based on spectator type (higher number = higher priority)
        if (type == "VIP") priority = 5;
        else if (type == "Influencer") priority = 4;
        else if (type == "General") priority = 3;
        else priority = 1;
    }
    
    void displayInfo() const {
        cout << "ID: " << spectatorId << " | Name: " << name 
             << " | Type: " << spectatorType << " | Priority: " << priority
             << " | Arrival: " << arrivalTime << " | Seated: " << (isSeated ? "Yes" : "No") << endl;
    }
};

// Streaming Slot class for managing live streaming setups
class StreamingSlot {
public:
    int slotNumber;
    string streamerName;
    string platform; // "Twitch", "YouTube", "Facebook", etc.
    string timeSlot;
    bool isActive;
    int viewerCount;
    
    StreamingSlot() : slotNumber(0), streamerName(""), platform(""), 
                      timeSlot(""), isActive(false), viewerCount(0) {}
    
    StreamingSlot(int slot, const string& name, const string& plat, const string& time)
        : slotNumber(slot), streamerName(name), platform(plat), timeSlot(time), 
          isActive(false), viewerCount(0) {}
    
    void displayInfo() const {
        cout << "Slot " << slotNumber << ": " << streamerName 
             << " (" << platform << ") | Time: " << timeSlot
             << " | Active: " << (isActive ? "Yes" : "No")
             << " | Viewers: " << viewerCount << endl;
    }
};

// Forward declaration for the complete demo function
class SpectatorManager;
void runCompleteDemo(SpectatorManager& manager);

// ============================================================================
// SPECTATOR MANAGER CLASS
// ============================================================================
class SpectatorManager 
{
    private:
        PriorityQueue<Spectator> priorityQueue;
        CircularQueue<Spectator> overflowQueue;
        Queue<StreamingSlot> streamingQueue;

        int totalSeatingCapacity;
        int currentSeatedCount;

        int totalSpectators;
        int totalVIPs;
        int totalInfluencers;
        int totalGeneral;
        int totalOverflow;

    public:
        SpectatorManager(int totalCapacity = 200, int overflowCapacity = 50);

        void addSpectator(const Spectator& spectator);
        void processSeating();
        void displayPriorityQueue();
        void processOverflowQueue();
        void addStreamingSlot(const StreamingSlot& slot);
        void activateNextStream();
        void displayStreamingQueue();
        void removeSpectator(const string& spectatorId);
        void displaySystemStats();
        void saveDataToFile(const string& filename);
        void loadHardcodedData();
        void runTask();
};

// Constructor implementation
SpectatorManager::SpectatorManager(int totalCapacity, int overflowCapacity) 
    : overflowQueue(overflowCapacity), totalSeatingCapacity(totalCapacity), 
      currentSeatedCount(0), totalSpectators(0), totalVIPs(0), 
      totalInfluencers(0), totalGeneral(0), totalOverflow(0) {}

// Add spectator to priority queue
void SpectatorManager::addSpectator(const Spectator& spectator) {
    priorityQueue.enqueue(spectator);
    
    // Update statistics
    if (spectator.spectatorType == "VIP") totalVIPs++;
    else if (spectator.spectatorType == "Influencer") totalInfluencers++;
    else totalGeneral++;
    
    totalSpectators++;
    
    cout << "Added " << spectator.spectatorType << " spectator: " << spectator.name 
         << " (ID: " << spectator.spectatorId << ") to priority queue" << endl;
    cout << "Priority Level: " << spectator.priority 
         << " (VIP=5, Influencer=4, General=3)" << endl;
}

// Process seating from priority queue (VIPs first, then Influencers, then General)
void SpectatorManager::processSeating() {
    cout << "\n=== PROCESSING SEATING (PRIORITY-BASED SYSTEM) ===" << endl;
    cout << "Processing Order: VIP -> Influencer -> General (within same priority: FIFO)" << endl;
    
    int seatedCount = 0;
    while (!priorityQueue.isEmpty() && currentSeatedCount < totalSeatingCapacity) {
        Spectator spectator = priorityQueue.dequeue();
        spectator.isSeated = true;
        currentSeatedCount++;
        seatedCount++;
        
        cout << "Seated #" << seatedCount << ": " << spectator.name 
             << " (" << spectator.spectatorType << ", Priority: " << spectator.priority 
             << ") - Arrival: " << spectator.arrivalTime << endl;
    }
    
    // Move remaining spectators to overflow queue
    while (!priorityQueue.isEmpty()) {
        Spectator spectator = priorityQueue.dequeue();
        overflowQueue.enqueue(spectator);
        totalOverflow++;
        cout << "Moved to overflow: " << spectator.name 
             << " (" << spectator.spectatorType << ")" << endl;
    }
    
    if (seatedCount > 0) {
        cout << "Successfully seated " << seatedCount << " spectators using priority system." << endl;
        cout << "VIPs and Influencers were given priority seating!" << endl;
    } else {
        cout << "No spectators to seat or venue is full." << endl;
    }
}

// Display priority queue contents
void SpectatorManager::displayPriorityQueue() {
    cout << "\n=== PRIORITY SPECTATOR QUEUE ===" << endl;
    cout << "Queue Order: Priority-based (VIP->Influencer->General, FIFO within same priority)" << endl;
    if (priorityQueue.isEmpty()) {
        cout << "Priority queue is empty." << endl;
    } else {
        cout << "Current priority queue:" << endl;
        priorityQueue.displayAll();
    }
}

// Manage overflow queue - implement waiting list system
void SpectatorManager::processOverflowQueue() {
    cout << "\n=== PROCESSING OVERFLOW QUEUE ===" << endl;
    
    if (overflowQueue.isEmpty()) {
        cout << "No spectators in overflow queue." << endl;
        return;
    }
    
    cout << "Spectators in waiting list (Overflow Queue - FIFO order):" << endl;
    overflowQueue.displayAll();
    
    cout << "\nNote: When seats become available, overflow spectators will be processed in FIFO order." << endl;
}

// Add streaming slot to queue
void SpectatorManager::addStreamingSlot(const StreamingSlot& slot) {
    streamingQueue.enqueue(slot);
    cout << "Added streaming slot: " << slot.streamerName 
         << " on " << slot.platform << endl;
}

// Activate next streaming slot from queue
void SpectatorManager::activateNextStream() {
    if (streamingQueue.isEmpty()) {
        cout << "No streaming slots available." << endl;
        return;
    }
    
    StreamingSlot slot = streamingQueue.dequeue();
    slot.isActive = true;
    slot.viewerCount = 150 + (rand() % 500); // Simulate viewer count
    
    cout << "LIVE: " << slot.streamerName << " streaming on " 
         << slot.platform << " with " << slot.viewerCount << " viewers!" << endl;
    
    cout << "Remaining streaming slots in queue: " << streamingQueue.size() << endl;
}

// Display current streaming queue
void SpectatorManager::displayStreamingQueue() {
    cout << "\n=== STREAMING QUEUE ===" << endl;
    
    if (streamingQueue.isEmpty()) {
        cout << "No streaming slots in queue." << endl;
        return;
    }
    
    cout << "Upcoming streaming slots (FIFO order):" << endl;
    streamingQueue.displayAll();
}

// Handle spectator leaving (free up seat) - priority for overflow
void SpectatorManager::removeSpectator(const string& spectatorId) {
    cout << "Processing spectator departure: " << spectatorId << endl;
    
    if (currentSeatedCount > 0) {
        currentSeatedCount--;
        
        // Try to seat someone from overflow queue (FIFO from overflow)
        if (!overflowQueue.isEmpty()) {
            Spectator nextSpectator = overflowQueue.dequeue();
            nextSpectator.isSeated = true;
            currentSeatedCount++;
            totalOverflow--;
            
            cout << "Seated from overflow queue: " << nextSpectator.name 
                 << " (" << nextSpectator.spectatorType << ")" << endl;
        }
    }
}

// Display comprehensive system statistics
void SpectatorManager::displaySystemStats() {
    cout << "\n" << string(70, '=') << endl;
    cout << "    SPECTATOR MANAGEMENT SYSTEM STATISTICS (PRIORITY QUEUE)" << endl;
    cout << string(70, '=') << endl;
    
    cout << "SEATING CAPACITY:" << endl;
    cout << "  Total Capacity: " << totalSeatingCapacity << " seats" << endl;
    cout << "  Current Seated: " << currentSeatedCount << " spectators" << endl;
    cout << "  Available Seats: " << (totalSeatingCapacity - currentSeatedCount) << endl;
    
    cout << "\nSPECTATOR BREAKDOWN:" << endl;
    cout << "  Total Registered: " << totalSpectators << endl;
    cout << "  VIPs (Priority 5): " << totalVIPs << endl;
    cout << "  Influencers (Priority 4): " << totalInfluencers << endl;
    cout << "  General (Priority 3): " << totalGeneral << endl;
    cout << "  In Overflow Queue: " << totalOverflow << endl;
    
    cout << "\nQUEUE STATUS:" << endl;
    cout << "  Priority Queue: " << priorityQueue.size() << " waiting" << endl;
    cout << "  Overflow Queue: " << overflowQueue.size() << " waiting" << endl;
    cout << "  Streaming Queue: " << streamingQueue.size() << " pending" << endl;
    
    cout << "\nPRIORITY SYSTEM:" << endl;
    cout << "  VIP (Priority 5): Premium seating first" << endl;
    cout << "  Influencer (Priority 4): Priority after VIPs" << endl;
    cout << "  General (Priority 3): Standard processing" << endl;
    cout << "  Within same priority: First-Come-First-Served" << endl;
    
    double occupancyRate = (totalSpectators > 0) ? 
        (double)currentSeatedCount / totalSeatingCapacity * 100 : 0;
    cout << "\nOCCUPANCY RATE: " << occupancyRate << "%" << endl;
    cout << string(70, '=') << endl;
}

// Save system data to CSV file
void SpectatorManager::saveDataToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot create file " << filename << endl;
        return;
    }
    
    file << "SpectatorType,Count,Percentage,Priority" << endl;
    if (totalSpectators > 0) {
        file << "VIP," << totalVIPs << "," << (double)totalVIPs/totalSpectators*100 << ",5" << endl;
        file << "Influencer," << totalInfluencers << "," << (double)totalInfluencers/totalSpectators*100 << ",4" << endl;
        file << "General," << totalGeneral << "," << (double)totalGeneral/totalSpectators*100 << ",3" << endl;
        file << "Overflow," << totalOverflow << "," << (double)totalOverflow/totalSpectators*100 << ",0" << endl;
    }
    
    file.close();
    cout << "System data saved to " << filename << endl;
}

// Load hardcoded sample data for demonstration
void SpectatorManager::loadHardcodedData() {
    cout << "Loading sample spectator data for priority queue demonstration..." << endl;
    cout << "Adding in mixed arrival order to show priority queue sorting..." << endl;
    
    // Add spectators in mixed order to demonstrate priority queue sorting
    addSpectator(Spectator("Michael Brown", "GEN001", "General", "09:00"));
    addSpectator(Spectator("Dr. Sarah Chen", "VIP001", "VIP", "09:15"));
    addSpectator(Spectator("Anna Wilson", "GEN002", "General", "09:30"));
    addSpectator(Spectator("GameMaster_Pro", "INF001", "Influencer", "09:45"));
    addSpectator(Spectator("Robert Taylor", "GEN003", "General", "10:00"));
    addSpectator(Spectator("Prof. Ahmad Rahman", "VIP002", "VIP", "10:15"));
    addSpectator(Spectator("EsportsQueen", "INF002", "Influencer", "10:30"));
    addSpectator(Spectator("Ms. Emily Watson", "VIP003", "VIP", "10:45"));
    addSpectator(Spectator("TechReviewer", "INF003", "Influencer", "11:00"));
    addSpectator(Spectator("John Smith", "GEN004", "General", "11:15"));
    
    // Add streaming slots
    addStreamingSlot(StreamingSlot(1, "ProGamer_Stream", "Twitch", "14:00-15:00"));
    addStreamingSlot(StreamingSlot(2, "EsportsLive", "YouTube", "15:00-16:00"));
    addStreamingSlot(StreamingSlot(3, "GameCaster_Official", "Facebook", "16:00-17:00"));
    addStreamingSlot(StreamingSlot(4, "APU_Esports", "Twitch", "17:00-18:00"));
    
    cout << "\nSample data loaded successfully!" << endl;
    cout << "Priority Order: All VIPs first, then Influencers, then General spectators" << endl;
}

// ============================================================================
// MAIN PROGRAM AND MENU SYSTEM
// ============================================================================

void displayMainMenu() {
    cout << string(70, '=') << endl;
    cout << "1.  Add New Spectator (Priority Queue)" << endl;
    cout << "2.  Display Priority Queue" << endl;
    cout << "3.  Process Priority Seating (VIP -> Influencer -> General)" << endl;
    cout << "4.  Manage Circular Overflow Queue" << endl;
    cout << "5.  Add Streaming Slot (Regular Queue)" << endl;
    cout << "6.  Activate Next Stream" << endl;
    cout << "7.  Display Streaming Queue" << endl;
    cout << "8.  Remove Spectator (Departure)" << endl;
    cout << "9.  Display System Statistics" << endl;
    cout << "10. Load Sample Data" << endl;
    cout << "11. Save Data to File" << endl;
    cout << "12. Run Complete Queue Demo" << endl;
    cout << "0.  Exit System" << endl;
    cout << string(70, '=') << endl;
    cout << "Enter your choice: ";
}

void runCompleteDemo(SpectatorManager& manager) {
    cout << "\nRUNNING COMPLETE QUEUE SYSTEM DEMO" << endl;
    cout << "Demonstrating: Priority Queue + Circular Queue + Regular Queue" << endl;
    cout << string(75, '=') << endl;
    
    // Step 1: Load sample data
    cout << "\n[STEP 1] Loading sample spectator data..." << endl;
    manager.loadHardcodedData();
    
    // Step 2: Display priority queue
    cout << "\n[STEP 2] Display priority queue (VIP->Influencer->General):" << endl;
    manager.displayPriorityQueue();
    
    // Step 3: Display initial statistics
    cout << "\n[STEP 3] Initial system state:" << endl;
    manager.displaySystemStats();
    
    // Step 4: Process seating using Priority Queue
    cout << "\n[STEP 4] Processing seating using Priority Queue..." << endl;
    manager.processSeating();
    
    // Step 5: Handle circular overflow
    cout << "\n[STEP 5] Managing circular overflow queue (prevents congestion)..." << endl;
    manager.processOverflowQueue();
    
    // Step 6: Display streaming queue
    cout << "\n[STEP 6] Display streaming queue (FIFO organization)..." << endl;
    manager.displayStreamingQueue();
    
    // Step 7: Activate streaming
    cout << "\n[STEP 7] Activating live streams from queue..." << endl;
    manager.activateNextStream();
    manager.activateNextStream();
    
    // Step 8: Simulate spectator departure
    cout << "\n[STEP 8] Simulating spectator departure..." << endl;
    manager.removeSpectator("VIP001");
    manager.removeSpectator("INF001");
    
    // Step 9: Final statistics
    cout << "\n[STEP 9] Final system statistics:" << endl;
    manager.displaySystemStats();
    
    // Step 10: Save data
    cout << "\n[STEP 10] Saving system data..." << endl;
    manager.saveDataToFile("queue_system_statistics.csv");
    
    cout << "\nQUEUE SYSTEM DEMO COMPLETED SUCCESSFULLY!" << endl;
    cout << "All three queue types working together seamlessly" << endl;
    cout << string(75, '=') << endl;
}

void SpectatorManager::runTask() {
    cout << "\n[Task 3] Live Stream & Spectator Queue Management\n" << endl;
    SpectatorManager manager(7, 15); // 7 seats, 15 overflow capacity for demo
    int choice;
    
    do {
        displayMainMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "\n=== ADD NEW SPECTATOR (Priority Queue) ===" << endl;
                string name, id, type, time;
                cout << "Enter spectator name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter spectator ID: ";
                cin >> id;
                cout << "Enter type (VIP/Influencer/General): ";
                cin >> type;
                cout << "Enter arrival time (HH:MM): ";
                cin >> time;
                
                Spectator newSpectator(name, id, type, time);
                manager.addSpectator(newSpectator);
                break;
            }
            case 2:
                manager.displayPriorityQueue();
                break;
            case 3:
                manager.processSeating();
                break;
            case 4:
                manager.processOverflowQueue();
                break;
            case 5: {
                cout << "\n=== ADD STREAMING SLOT (Regular Queue) ===" << endl;
                int slotNum;
                string streamer, platform, timeSlot;
                cout << "Enter slot number: ";
                cin >> slotNum;
                cout << "Enter streamer name: ";
                cin.ignore();
                getline(cin, streamer);
                cout << "Enter platform (Twitch/YouTube/Facebook): ";
                cin >> platform;
                cout << "Enter time slot (e.g., 14:00-15:00): ";
                cin >> timeSlot;
                
                StreamingSlot slot(slotNum, streamer, platform, timeSlot);
                manager.addStreamingSlot(slot);
                break;
            }
            case 6:
                manager.activateNextStream();
                break;
            case 7:
                manager.displayStreamingQueue();
                break;
            case 8: {
                cout << "\n=== REMOVE SPECTATOR ===" << endl;
                string spectatorId;
                cout << "Enter spectator ID to remove: ";
                cin >> spectatorId;
                manager.removeSpectator(spectatorId);
                break;
            }
            case 9:
                manager.displaySystemStats();
                break;
            case 10:
                manager.loadHardcodedData();
                break;
            case 11:
                manager.saveDataToFile("queue_system_report.csv");
                break;
            case 12:
                runCompleteDemo(manager);
                break;
            case 0:
                cout << "Exiting system..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}