#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "structs.h"
#include "helper.h"
#include "CircularQueue.h"
#include "PriorityQueue.h"

using namespace std;

// ============================================================================
// SPECTATOR AND STREAMING CLASSES
// ============================================================================

// Enhanced Spectator class
class Spectator {
public:
    string name;
    string spectatorId;
    string spectatorType; // "VIP", "Influencer", "General"
    string viewingMode;   // "Physical", "Online" - ADDED MISSING MEMBER
    int priority;
    string arrivalTime;
    bool isSeated;
    
    Spectator() : name(""), spectatorId(""), spectatorType("General"), 
                  viewingMode("Physical"), priority(3), arrivalTime(""), isSeated(false) {}
    
    Spectator(const string& n, const string& id, const string& type, const string& time) 
        : name(n), spectatorId(id), spectatorType(type), viewingMode("Physical"), 
          arrivalTime(time), isSeated(false) {
        // Assign priority based on spectator type (lower number = higher priority)
        if (type == "VIP") priority = 1;
        else if (type == "Influencer") priority = 2;
        else if (type == "General") priority = 3;
        else priority = 3;
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

// ============================================================================
// ENHANCED SPECTATOR MANAGER CLASS
// ============================================================================

class SpectatorManager {
private:
    // Original audience system
    Audience audience[100];
    int audienceCount;
    
    // Enhanced queue systems
    PriorityQueue spectatorPriorityQueue;    // For VIP/Influencer/General priority
    CircularQueue seatingQueue;              // For seating management
    CircularQueue overflowQueue;             // For overflow spectators
    
    // Capacity management
    int maxTotalPhysicalCapacity;     // 200 total physical seats
    int maxVIPCapacity;               // 20 VIP physical seats
    int maxInfluencerCapacity;        // 30 Influencer physical seats
    int maxOnlineCapacity;   
    
    // Current counts
    int currentPhysicalSeated;
    int currentVIPSeated;
    int currentInfluencerSeated;
    int currentOnlineViewers;
    
    // Statistics tracking
    int totalSpectators;
    int totalVIPs;
    int totalInfluencers;
    int totalGeneral;
    int totalOverflow;
    
    void displayMenu() {
        cout << "\n" << string(70, '=') << endl;
        cout << "    LIVE STREAM & SPECTATOR QUEUE MANAGEMENT" << endl;
        cout << string(70, '=') << endl;
        cout << "1.  Buy Ticket & Join Audience" << endl;
        cout << "2.  Seat Audience Member" << endl;
        cout << "3.  View All Audience Members" << endl;
        cout << "4.  Display Priority Queue" << endl;
        cout << "5.  Process Priority Seating (VIP -> Influencer -> General)" << endl;
        cout << "6.  Manage Circular Overflow Queue" << endl;
        cout << "7.  Remove Spectator (Departure)" << endl;
        cout << "8.  Manage Overflow Audience" << endl;
        cout << "9. View Capacity Status" << endl;
        cout << "10. Add All Existing Audience to Priority Queue" << endl;
        
        cout << "\n=== EXIT ===" << endl;
        cout << "11. Back to Main Menu" << endl;
        cout << string(70, '-') << endl;
        cout << "Enter your choice: ";
    }
    
    // ========================================================================
    // ENHANCED LIVESTREAM AND SPECTATOR MANAGEMENT METHODS
    // ========================================================================
    
    void buyTicketJoinAudience() {
    if (audienceCount >= 100) {
        cout << "Maximum audience capacity reached!" << endl;
        return;
    }
    
    Audience newAudience;
    cout << "\n=== TICKET PURCHASE & AUDIENCE REGISTRATION ===" << endl;
    
    newAudience.audienceID = Utils::generateAudienceID(audienceCount);
    
    cout << "Audience ID: " << newAudience.audienceID << endl;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, newAudience.name);
    
    cout << "Enter email: ";
    getline(cin, newAudience.email);
    
    // Get viewing mode preference first
    cout << "Select Viewing Mode:" << endl;
    cout << "1. Physical Attendance" << endl;
    cout << "2. Online Streaming" << endl;
    cout << "Enter choice: ";
    
    int viewingChoice;
    cin >> viewingChoice;
    
    string viewingMode = (viewingChoice == 1) ? "Physical" : "Online";
    
    int ticketChoice;
    
    // If online streaming, only allow general tickets
    if (viewingChoice == 2) {
        cout << "\nFor Online Streaming, only General tickets are available." << endl;
        cout << "General Ticket (Priority: 3) - Auto-selected" << endl;
        ticketChoice = 3;
    } else {
        // For physical attendance, show all options
        cout << "Select Ticket Type:" << endl;
        cout << "1. VIP (Priority: 1)" << endl;
        cout << "2. Influencer/Streamer (Priority: 2)" << endl;
        cout << "3. General (Priority: 3)" << endl;
        cout << "Enter choice: ";
        cin >> ticketChoice;
    }
    
    switch (ticketChoice) {
        case 1:
            newAudience.ticketType = "VIP";
            newAudience.priority = 1;
            newAudience.seatNumber = "VIP-" + to_string(audienceCount + 1);
            totalVIPs++;
            break;
        case 2:
            newAudience.ticketType = "Influencer";
            newAudience.priority = 2;
            newAudience.seatNumber = "INF-" + to_string(audienceCount + 1);
            totalInfluencers++;
            break;
        case 3:
            newAudience.ticketType = "General";
            newAudience.priority = 3;
            newAudience.seatNumber = "GEN-" + to_string(audienceCount + 1);
            totalGeneral++;
            break;
        default:
            cout << "Invalid choice! Setting as General ticket." << endl;
            newAudience.ticketType = "General";
            newAudience.priority = 3;
            newAudience.seatNumber = "GEN-" + to_string(audienceCount + 1);
            totalGeneral++;
            break;
    }
    
    newAudience.isSeated = false;
    
    // Store audience member
    audience[audienceCount] = newAudience;
    totalSpectators++;
    
    // FIXED: Add to priority queue with proper format
    string spectatorInfo = newAudience.audienceID + " " + newAudience.name + 
                          " (" + newAudience.ticketType + ", " + viewingMode + ")";
    spectatorPriorityQueue.enqueue(spectatorInfo, newAudience.priority);
    
    // Also add to seating queue for legacy compatibility
    seatingQueue.enqueue(newAudience.audienceID[3]); // Use last character of ID
    
    audienceCount++;
    
    cout << "\n=== TICKET PURCHASE SUCCESSFUL ===" << endl;
    cout << "Audience ID: " << newAudience.audienceID << endl;
    cout << "Name: " << newAudience.name << endl;
    cout << "Ticket Type: " << newAudience.ticketType << " (Priority: " << newAudience.priority << ")" << endl;
    cout << "Viewing Mode: " << viewingMode << endl;
    cout << "Assigned Seat: " << newAudience.seatNumber << endl;
    cout << "Status: Added to priority queue for seating" << endl;
    
    FileManager::saveAudience(audience, audienceCount);
    
    cout << "\nCurrent Queue Status:" << endl;
    cout << "Total in priority queue: " << totalSpectators << endl;
    cout << "VIPs: " << totalVIPs << ", Influencers: " << totalInfluencers << ", General: " << totalGeneral << endl;
    }
    
    void seatAudienceMember() {
        string audienceID;
        cout << "\n=== SEAT AUDIENCE MEMBER (LEGACY METHOD) ===" << endl;
        cout << "Enter Audience ID: ";
        cin >> audienceID;
        
        int index = Utils::findAudienceIndex(audience, audienceCount, audienceID);
        if (index == -1) {
            cout << "Audience member not found!" << endl;
            return;
        }
        
        if (audience[index].isSeated) {
            cout << "Audience member " << audience[index].name << " is already seated!" << endl;
            return;
        }
        
        bool canSeat = false;
        if (audience[index].ticketType == "VIP") {
            if (currentVIPSeated < maxVIPCapacity && currentPhysicalSeated < maxTotalPhysicalCapacity) {
                currentVIPSeated++;
                currentPhysicalSeated++;
                canSeat = true;
            }
        } else if (audience[index].ticketType == "Influencer") {
            if (currentInfluencerSeated < maxInfluencerCapacity && currentPhysicalSeated < maxTotalPhysicalCapacity) {
                currentInfluencerSeated++;
                currentPhysicalSeated++;
                canSeat = true;
            }
        } else {
            // General audience
            int remainingPhysical = maxTotalPhysicalCapacity - currentPhysicalSeated;
            int reservedVIP = maxVIPCapacity - currentVIPSeated;
            int reservedInfluencer = maxInfluencerCapacity - currentInfluencerSeated;
            int availableGeneral = remainingPhysical - reservedVIP - reservedInfluencer;
            
            if (availableGeneral > 0) {
                currentPhysicalSeated++;
                canSeat = true;
            }
        }
        
        if (!canSeat) {
            cout << "Cannot seat audience member - capacity reached for " << audience[index].ticketType << " type!" << endl;
            cout << "Consider online viewing option." << endl;
            return;
        }

        audience[index].isSeated = true;
        
        cout << "Audience member " << audience[index].name << " seated successfully!" << endl;
        cout << "Seat Number: " << audience[index].seatNumber << endl;
        cout << "Ticket Type: " << audience[index].ticketType << endl;
        cout << "Seating time: " << Utils::getCurrentTimestamp() << endl;
        
        FileManager::saveAudience(audience, audienceCount);
    }
    
    void viewAllAudience() {
        cout << "\n=== ALL AUDIENCE MEMBERS ===" << endl;
        
        if (audienceCount == 0) {
            cout << "No audience members registered yet." << endl;
            return;
        }
        
        cout << left << setw(8) << "ID" 
             << setw(20) << "Name" 
             << setw(25) << "Email" 
             << setw(12) << "Ticket Type" 
             << setw(10) << "Priority" 
             << setw(12) << "Seat Number" 
             << setw(8) << "Seated" << endl;
        cout << string(95, '-') << endl;
        
        for (int i = 0; i < audienceCount; i++) {
            cout << left << setw(8) << audience[i].audienceID
                 << setw(20) << audience[i].name
                 << setw(25) << audience[i].email
                 << setw(12) << audience[i].ticketType
                 << setw(10) << audience[i].priority
                 << setw(12) << audience[i].seatNumber
                 << setw(8) << (audience[i].isSeated ? "Yes" : "No") << endl;
        }
        
        cout << "\nQueue Statistics:" << endl;
        cout << "Total Spectators: " << totalSpectators << endl;
        cout << "In Priority Queue: " << (spectatorPriorityQueue.isEmpty() ? 0 : totalSpectators) << endl;
        cout << "VIPs: " << totalVIPs << ", Influencers: " << totalInfluencers << ", General: " << totalGeneral << endl;
    }

    void displayPriorityQueue() {
        cout << "\n=== PRIORITY SPECTATOR QUEUE ===" << endl;
        cout << "Queue Order: Priority-based (VIP -> Influencer -> General, FIFO within same priority)" << endl;
        if (spectatorPriorityQueue.isEmpty()) {
            cout << "Priority queue is empty." << endl;
            cout << "Total registered audience: " << audienceCount << endl;
            if (audienceCount > 0) {
                cout << "Use option 10 to add all existing audience to the priority queue." << endl;
            }
        }
        else {
            cout << "Current priority queue contents:" << endl;
            spectatorPriorityQueue.display();
            cout << "\nQueue Statistics:" << endl;
            cout << "Total in queue: " << totalSpectators << endl;
            cout << "VIPs: " << totalVIPs << ", Influencers: " << totalInfluencers << ", General: " << totalGeneral << endl;
        }
    }

    void processPrioritySeating() {
        cout << "\n=== PROCESSING PRIORITY SEATING (VIP → Influencer → General) ===" << endl;
        
        if (spectatorPriorityQueue.isEmpty()) {
            cout << "Priority queue is empty!" << endl;
            if (audienceCount > 0) {
                cout << "There are " << audienceCount << " registered audience members." << endl;
                cout << "Use option 10 to add them to the priority queue first." << endl;
            }
            return;
        }
        
        cout << "Processing Order: VIP -> Influencer -> General (within same priority: FIFO)" << endl;
        cout << "Checking capacity limits for each type..." << endl;

        int seatedCount = 0;
        int processedCount = 0;
        
        while (!spectatorPriorityQueue.isEmpty()) {
            string spectatorInfo = spectatorPriorityQueue.dequeue();
            processedCount++;
            
            // Parse spectator info to determine type and viewing mode
            bool isVIP = spectatorInfo.find("(VIP,") != string::npos;
            bool isInfluencer = spectatorInfo.find("(Influencer,") != string::npos;
            bool isPhysical = spectatorInfo.find("Physical)") != string::npos;
            bool isOnline = spectatorInfo.find("Online)") != string::npos;
            
            bool canSeat = false;
            string seatType = "";
            
            if (isPhysical) {
                // Check physical capacity limits
                if (currentPhysicalSeated < maxTotalPhysicalCapacity) {
                    if (isVIP && currentVIPSeated < maxVIPCapacity) {
                        currentVIPSeated++;
                        currentPhysicalSeated++;
                        canSeat = true;
                        seatType = "VIP Physical";
                    } else if (isInfluencer && currentInfluencerSeated < maxInfluencerCapacity) {
                        currentInfluencerSeated++;
                        currentPhysicalSeated++;
                        canSeat = true;
                        seatType = "Influencer Physical";
                    } else if (!isVIP && !isInfluencer) {
                        // General audience - check remaining physical capacity
                        int remainingPhysical = maxTotalPhysicalCapacity - currentPhysicalSeated;
                        int reservedVIP = maxVIPCapacity - currentVIPSeated;
                        int reservedInfluencer = maxInfluencerCapacity - currentInfluencerSeated;
                        int availableGeneral = remainingPhysical - reservedVIP - reservedInfluencer;
                        
                        if (availableGeneral > 0) {
                            currentPhysicalSeated++;
                            canSeat = true;
                            seatType = "General Physical";
                        }
                    }
                }
            } else if (isOnline) {
                // Check online capacity
                if (currentOnlineViewers < maxOnlineCapacity) {
                    currentOnlineViewers++;
                    canSeat = true;
                    seatType = "Online";
                }
            }
            
            if (canSeat) {
                seatedCount++;
                totalSpectators--;
                
                // Update statistics
                if (isVIP) totalVIPs--;
                else if (isInfluencer) totalInfluencers--;
                else totalGeneral--;
                
                // Mark as seated in audience array
                string audienceID = spectatorInfo.substr(0, spectatorInfo.find(' '));
                int index = Utils::findAudienceIndex(audience, audienceCount, audienceID);
                if (index != -1) {
                    audience[index].isSeated = true;
                }
                
                cout << "Seated #" << seatedCount << " (" << seatType << "): " << spectatorInfo << endl;
            } else {
                // Move to overflow queue
                char spectatorChar = spectatorInfo[0];
                overflowQueue.enqueue(spectatorChar);
                totalOverflow++;
                cout << "-> Moved to overflow: " << spectatorInfo << " (Capacity reached)" << endl;
            }
        }
        
        cout << "\n=== SEATING PROCESS COMPLETE ===" << endl;
        cout << "Processed: " << processedCount << " spectators" << endl;
        cout << "Successfully seated: " << seatedCount << " spectators" << endl;
        cout << "Moved to overflow: " << totalOverflow << " spectators" << endl;
        
        if (seatedCount > 0) {
            cout << "VIPs and Influencers were given priority seating!" << endl;
            FileManager::saveAudience(audience, audienceCount);
        }
        
        cout << "\nUpdated Capacity Status:" << endl;
        cout << "Physical: " << currentPhysicalSeated << "/" << maxTotalPhysicalCapacity << endl;
        cout << "VIP: " << currentVIPSeated << "/" << maxVIPCapacity << endl;
        cout << "Influencer: " << currentInfluencerSeated << "/" << maxInfluencerCapacity << endl;
        cout << "Online: " << currentOnlineViewers << "/" << maxOnlineCapacity << endl;
    }
    
    void manageCircularOverflowQueue() {
        cout << "\n=== MANAGE CIRCULAR OVERFLOW QUEUE ===" << endl;
        
        if (overflowQueue.isEmpty()) {
            cout << "No spectators in overflow queue." << endl;
            return;
        }
        
        cout << "Spectators in waiting list (Overflow Queue - FIFO order):" << endl;
        overflowQueue.display();
        cout << "Total in overflow: " << totalOverflow << endl;
        
        cout << "\nOverflow Management Options:" << endl;
        cout << "1. Process next overflow spectator" << endl;
        cout << "2. View overflow queue status" << endl;
        cout << "3. Clear overflow queue" << endl;
        cout << "4. Process all overflow spectators" << endl;
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                if (!overflowQueue.isEmpty()) {
                    char nextSpectator = overflowQueue.dequeue();
                    totalOverflow--;
                    
                    // Try to seat based on available capacity
                    bool seated = false;
                    if (currentPhysicalSeated < maxTotalPhysicalCapacity) {
                        currentPhysicalSeated++;
                        seated = true;
                        cout << "Seated from overflow: Spectator ID starting with '" 
                             << nextSpectator << "' (Physical)" << endl;
                    } else if (currentOnlineViewers < maxOnlineCapacity) {
                        currentOnlineViewers++;
                        seated = true;
                        cout << "Seated from overflow: Spectator ID starting with '" 
                             << nextSpectator << "' (Online)" << endl;
                    }
                    
                    if (!seated) {
                        // Put back in overflow if still no capacity
                        overflowQueue.enqueue(nextSpectator);
                        totalOverflow++;
                        cout << "Cannot process: All capacities still full." << endl;
                    }
                } else {
                    cout << "Cannot process: Queue empty." << endl;
                }
                break;
            case 2:
                cout << "Overflow queue size: " << totalOverflow << endl;
                cout << "Physical capacity: " << currentPhysicalSeated << "/" << maxTotalPhysicalCapacity << endl;
                cout << "Online capacity: " << currentOnlineViewers << "/" << maxOnlineCapacity << endl;
                cout << "VIP capacity: " << currentVIPSeated << "/" << maxVIPCapacity << endl;
                cout << "Influencer capacity: " << currentInfluencerSeated << "/" << maxInfluencerCapacity << endl;
                break;
            case 3:
                {
                    int cleared = 0;
                    while (!overflowQueue.isEmpty()) {
                        overflowQueue.dequeue();
                        cleared++;
                    }
                    totalOverflow = 0;
                    cout << "Overflow queue cleared. Removed " << cleared << " spectators." << endl;
                }
                break;
            case 4:
                {
                    int processedFromOverflow = 0;
                    while (!overflowQueue.isEmpty() && 
                           (currentPhysicalSeated < maxTotalPhysicalCapacity || 
                            currentOnlineViewers < maxOnlineCapacity)) {
                        
                        char nextSpectator = overflowQueue.dequeue();
                        totalOverflow--;
                        processedFromOverflow++;
                        
                        if (currentPhysicalSeated < maxTotalPhysicalCapacity) {
                            currentPhysicalSeated++;
                            cout << "Seated from overflow: Spectator ID starting with '" 
                                 << nextSpectator << "' (Physical)" << endl;
                        } else if (currentOnlineViewers < maxOnlineCapacity) {
                            currentOnlineViewers++;
                            cout << "Seated from overflow: Spectator ID starting with '" 
                                 << nextSpectator << "' (Online)" << endl;
                        }
                    }
                    cout << "Processed " << processedFromOverflow << " spectators from overflow." << endl;
                    if (!overflowQueue.isEmpty()) {
                        cout << "Remaining in overflow: " << totalOverflow << endl;
                    }
                }
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    }
    
    void removeSpectator() {
        cout << "\n=== REMOVE SPECTATOR (DEPARTURE) ===" << endl;
        string spectatorId;
        cout << "Enter spectator ID to remove: ";
        cin >> spectatorId;
        
        // Find in audience array
        int index = Utils::findAudienceIndex(audience, audienceCount, spectatorId);
        if (index == -1) {
            cout << "Spectator not found in audience list!" << endl;
            return;
        }
        
        string type = audience[index].ticketType;
        bool wasSeated = audience[index].isSeated;
        
        cout << "Enter viewing mode for removal (Physical/Online): ";
        string mode;
        cin >> mode;
        
        cout << "Processing spectator departure: " << spectatorId << " (" << type << ", " << mode << ")" << endl;
        
        // Update capacity counters only if they were actually seated
        if (wasSeated) {
            if (mode == "Physical") {
                if (currentPhysicalSeated > 0) {
                    currentPhysicalSeated--;
                    if (type == "VIP" && currentVIPSeated > 0) {
                        currentVIPSeated--;
                    } else if (type == "Influencer" && currentInfluencerSeated > 0) {
                        currentInfluencerSeated--;
                    }
                }
            } else if (mode == "Online") {
                if (currentOnlineViewers > 0) {
                    currentOnlineViewers--;
                }
            }
            
            // Mark as not seated
            audience[index].isSeated = false;
        }
        
        // Try to seat someone from overflow queue
        if (!overflowQueue.isEmpty()) {
            char nextSpectator = overflowQueue.dequeue();
            totalOverflow--;
            
            // Try to seat based on available capacity
            if (mode == "Physical" && currentPhysicalSeated < maxTotalPhysicalCapacity) {
                currentPhysicalSeated++;
                cout << "Seated from overflow queue: Spectator ID starting with '" 
                     << nextSpectator << "' (Physical)" << endl;
            } else if (currentOnlineViewers < maxOnlineCapacity) {
                currentOnlineViewers++;
                cout << "Seated from overflow queue: Spectator ID starting with '" 
                     << nextSpectator << "' (Online)" << endl;
            } else {
                // Put back in overflow if no capacity
                overflowQueue.enqueue(nextSpectator);
                totalOverflow++;
            }
        }
        
        cout << "Spectator " << spectatorId << " removed successfully." << endl;
        FileManager::saveAudience(audience, audienceCount);
    }
    
    void addAllAudienceToQueue() {
        cout << "\n=== ADD ALL EXISTING AUDIENCE TO PRIORITY QUEUE ===" << endl;
        
        if (audienceCount == 0) {
            cout << "No audience members found to add to queue." << endl;
            return;
        }
        
        int addedCount = 0;
        cout << "Adding all registered audience members to priority queue..." << endl;
        
        // Reset statistics
        totalSpectators = 0;
        totalVIPs = 0;
        totalInfluencers = 0;
        totalGeneral = 0;
        
        for (int i = 0; i < audienceCount; i++) {
            if (!audience[i].isSeated) {  // Only add unseated members
                // Convert ticket type to spectator type
                string spectatorType = audience[i].ticketType;
                if (spectatorType == "Streamer") {
                    spectatorType = "Influencer";
                }
                
                // Default to Physical viewing mode (can be updated based on requirements)
                string viewingMode = "Physical";
                
                string spectatorInfo = audience[i].audienceID + " " + audience[i].name + 
                                      " (" + spectatorType + ", " + viewingMode + ")";
                
                spectatorPriorityQueue.enqueue(spectatorInfo, audience[i].priority);
                
                totalSpectators++;
                if (spectatorType == "VIP") totalVIPs++;
                else if (spectatorType == "Influencer") totalInfluencers++;
                else totalGeneral++;
                
                addedCount++;
                
                cout << "Added: " << spectatorInfo << " (Priority: " << audience[i].priority << ")" << endl;
            }
        }
        
        cout << "\n=== QUEUE POPULATION COMPLETE ===" << endl;
        cout << "Added " << addedCount << " audience members to priority queue." << endl;
        cout << "Total in queue: " << totalSpectators << endl;
        cout << "VIPs: " << totalVIPs << ", Influencers: " << totalInfluencers << ", General: " << totalGeneral << endl;
        
        if (addedCount == 0) {
            cout << "All audience members are already seated." << endl;
        }
    }
    
    void viewCapacityStatus() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    CAPACITY STATUS REPORT" << endl;
        cout << string(60, '=') << endl;
        
        cout << "PHYSICAL SEATING:" << endl;
        cout << "  Total Physical: " << currentPhysicalSeated << "/" << maxTotalPhysicalCapacity 
             << " (" << (maxTotalPhysicalCapacity > 0 ? (double)currentPhysicalSeated/maxTotalPhysicalCapacity*100 : 0) << "%)" << endl;
        cout << "  VIP Seats: " << currentVIPSeated << "/" << maxVIPCapacity 
             << " (" << (maxVIPCapacity > 0 ? (double)currentVIPSeated/maxVIPCapacity*100 : 0) << "%)" << endl;
        cout << "  Influencer Seats: " << currentInfluencerSeated << "/" << maxInfluencerCapacity 
             << " (" << (maxInfluencerCapacity > 0 ? (double)currentInfluencerSeated/maxInfluencerCapacity*100 : 0) << "%)" << endl;
        
        int generalPhysical = currentPhysicalSeated - currentVIPSeated - currentInfluencerSeated;
        int maxGeneralPhysical = maxTotalPhysicalCapacity - maxVIPCapacity - maxInfluencerCapacity;
        cout << "  General Seats: " << generalPhysical << "/" << maxGeneralPhysical 
             << " (" << (maxGeneralPhysical > 0 ? (double)generalPhysical/maxGeneralPhysical*100 : 0) << "%)" << endl;
        
        cout << "\nONLINE VIEWING:" << endl;
        cout << "  Online Viewers: " << currentOnlineViewers << "/" << maxOnlineCapacity 
             << " (" << (maxOnlineCapacity > 0 ? (double)currentOnlineViewers/maxOnlineCapacity*100 : 0) << "%)" << endl;
        
        cout << "\nQUEUE STATUS:" << endl;
        cout << "  In Priority Queue: " << totalSpectators << endl;
        cout << "  VIPs: " << totalVIPs << ", Influencers: " << totalInfluencers << ", General: " << totalGeneral << endl;
        cout << "  Waiting in Overflow: " << totalOverflow << endl;
        
        int totalCapacity = maxTotalPhysicalCapacity + maxOnlineCapacity;
        int totalOccupied = currentPhysicalSeated + currentOnlineViewers;
        cout << "\nOVERALL:" << endl;
        cout << "  Total Occupied: " << totalOccupied << "/" << totalCapacity 
             << " (" << (totalCapacity > 0 ? (double)totalOccupied/totalCapacity*100 : 0) << "%)" << endl;
        cout << "  Total Registered: " << audienceCount << endl;
        cout << string(60, '=') << endl;
    }

public:
    SpectatorManager() : seatingQueue(50), overflowQueue(50) { // Both circular queues with capacity of 50
        // Initialize all member variables
        audienceCount = 0;
        totalSpectators = 0;
        totalVIPs = 0;
        totalInfluencers = 0;
        totalGeneral = 0;
        totalOverflow = 0;
        
        // Initialize capacity limits
        maxTotalPhysicalCapacity = 200;
        maxVIPCapacity = 20;
        maxInfluencerCapacity = 30;
        maxOnlineCapacity = 1000;
        
        // Initialize current counts
        currentPhysicalSeated = 0;
        currentVIPSeated = 0;
        currentInfluencerSeated = 0;
        currentOnlineViewers = 0;
        
        FileManager::loadAudience(audience, audienceCount);
        
        // Count already seated audience members and update statistics
        for (int i = 0; i < audienceCount; i++) {
            if (audience[i].isSeated) {
                currentPhysicalSeated++;
                if (audience[i].ticketType == "VIP") {
                    currentVIPSeated++;
                } else if (audience[i].ticketType == "Streamer" || audience[i].ticketType == "Influencer") {
                    currentInfluencerSeated++;
                }
            }
        }
        
        cout << "\n=== SPECTATOR MANAGER INITIALIZED ===" << endl;
        cout << "Loaded " << audienceCount << " audience members from file." << endl;
        cout << "Currently seated: " << currentPhysicalSeated << "/" << maxTotalPhysicalCapacity << endl;
        if (audienceCount > 0) {
            cout << "Use option 11 to add all existing audience to the priority queue." << endl;
        }
    }
    
    void runTask() {
        int choice;
        
        do {
            displayMenu();
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            
            switch (choice) {
                case 1:
                    buyTicketJoinAudience();
                    break;
                case 2:
                    seatAudienceMember();
                    break;
                case 3:
                    viewAllAudience();
                    break;
                case 4:
                    displayPriorityQueue();
                    break;
                case 5:
                    processPrioritySeating();
                    break;
                case 6:
                    manageCircularOverflowQueue();
                    break;
                case 7:
                    removeSpectator();
                    break;
                case 8:
                    manageCircularOverflowQueue();
                    break;
                case 9:
                    viewCapacityStatus();
                    break;
                case 10:
                    addAllAudienceToQueue();
                    break;
                case 11:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    break;
            }
            
            if (choice != 11) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
            
        } while (choice != 11);
    }
};