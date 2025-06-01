#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "structs.h"
#include "helper.h"
#include "CircularQueue.h"
#include "PriorityQueue.h"

using namespace std;

// Enhanced Spectator class
class Spectator {
public:
    string name;
    string spectatorId;
    string spectatorType; // "VIP", "Influencer", "General", "Online"
    string viewingMode;   // "Physical", "Online"
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

// ENHANCED SPECTATOR MANAGER CLASS
class SpectatorManager {
private:
    // Original audience system
    Audience audience[1000];
    int audienceCount;
    
    // Enhanced queue systems
    PriorityQueue spectatorPriorityQueue;    // For VIP/Influencer/General priority
    CircularQueue spectatorCircularQueue;    // For storing data in circular queue
    CircularQueue seatingQueue;              // For seating management
    CircularQueue overflowQueue;             // For overflow spectators
    
    // ENHANCED CAPACITY MANAGEMENT - All variables properly defined
    int maxTotalCapacity;             // 300 total spectators (VIP + Influencer + General)
    int maxTotalPhysicalCapacity;     // 200 total physical seats
    int maxVIPCapacity;               // 20 VIP physical seats
    int maxInfluencerCapacity;        // 30 Influencer physical seats
    int maxGeneralCapacity;           // 150 general capacity (physical + online)
    int maxOnlineCapacity;            // 100 online capacity
    
    // ENHANCED CURRENT COUNTS - All variables properly defined
    int currentSpectators;            // Current total spectators (all types)
    int currentPhysicalSeated;        // Current physical attendees (renamed for consistency)
    int currentVIPSeated;             // Current VIP seated
    int currentInfluencerSeated;      // Current Influencer seated
    int currentGeneralSeated;         // Current General seated (physical + online)
    int currentOnlineViewers;         // Current online viewers
    
    // ENHANCED STATISTICS TRACKING - All variables properly defined
    int totalSpectators;              // Total in queues/registered
    int totalPhysicalSpectators;      // Total physical spectators
    int totalVIPs;                    // Total VIPs in system
    int totalInfluencers;             // Total Influencers in system
    int totalGeneral;                 // Total General in system
    int totalOnline;                  // Total Online viewers in system
    int totalOverflow;                // Total in overflow queue
    
    void displayAdminMenu() {
        cout << "\n" << string(70, '=') << endl;
        cout << "    LIVE STREAM & SPECTATOR QUEUE MANAGEMENT" << endl;
        cout << string(70, '=') << endl;
        cout << "1.  Seat Audience Member" << endl;
        cout << "2.  View All Audience Members" << endl;
        cout << "3.  Process Priority Seating (VIP -> Influencer -> General)" << endl;
        cout << "4.  Manage Circular Overflow Queue" << endl;
        cout << "5.  Remove Spectator (Departure)" << endl;
        cout << "6.  View Capacity Status" << endl;
        cout << "7.  Add All Existing Audience to Circular and Priority Queues" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << string(70, '-') << endl;
        cout << "Enter your choice: ";
    }

    void displayAudienceMenu() {
        cout << "\n" << string(70, '=') << endl;
        cout << "    LIVE STREAM & SPECTATOR QUEUE MANAGEMENT" << endl;
        cout << string(70, '=') << endl;
        cout << "1.  Buy Ticket & Join Audience" << endl;
        cout << "0.  Back to Main Menu" << endl;
        cout << string(70, '-') << endl;
        cout << "Enter your choice: ";
    }
    
    // Helper function to update all relevant counters
    void updateSpectatorCounts() {
        // Reset current counts
        currentSpectators = 0;
        currentPhysicalSeated = 0;
        currentVIPSeated = 0;
        currentInfluencerSeated = 0;
        currentGeneralSeated = 0;
        currentOnlineViewers = 0;
        
        // Count from audience array
        for (int i = 0; i < audienceCount; i++) {
            if (audience[i].isSeated) {
                currentSpectators++;
                
                if (audience[i].ticketType == "VIP") {
                    currentVIPSeated++;
                    currentPhysicalSeated++;
                } else if (audience[i].ticketType == "Influencer" || audience[i].ticketType == "Streamer") {
                    currentInfluencerSeated++;
                    currentPhysicalSeated++;
                } else if (audience[i].ticketType == "General") {
                    currentGeneralSeated++;
                    // Assume physical unless specified as online
                    currentPhysicalSeated++;
                } else if (audience[i].ticketType == "Online") {
                    currentOnlineViewers++;
                    currentGeneralSeated++; // Online viewers are general category
                }
            }
        }
        
        // Update total physical spectators
        totalPhysicalSpectators = currentPhysicalSeated;
        totalOnline = currentOnlineViewers;
    }
    
    void buyTicketJoinAudience() {
        // Check total capacity first
        if (currentSpectators >= maxTotalCapacity) {
            cout << "\n*** MAXIMUM TOTAL CAPACITY REACHED ***" << endl;
            cout << "Cannot accept new spectators. Total capacity: " << maxTotalCapacity << endl;
            return;
        }
        
        Audience newAudience;
        cout << "\n=== TICKET PURCHASE & AUTOMATIC QUEUE ASSIGNMENT ===" << endl;
        
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
            // Check online capacity
            if (currentOnlineViewers >= maxOnlineCapacity) {
                cout << "Online viewing capacity full (" << maxOnlineCapacity << "). Cannot proceed." << endl;
                return;
            }
            if (currentGeneralSeated >= maxGeneralCapacity) {
                cout << "General capacity full (" << maxGeneralCapacity << "). Cannot proceed." << endl;
                return;
            }
            
            cout << "\nFor Online Streaming, only General tickets are available." << endl;
            cout << "General Ticket (Priority: 3) - Auto-selected" << endl;
            ticketChoice = 3;
        } else {
            // For physical attendance, check physical capacity
            if (currentPhysicalSeated >= maxTotalPhysicalCapacity) {
                cout << "Physical attendance capacity full (" << maxTotalPhysicalCapacity << "). Consider online viewing." << endl;
                return;
            }
            
            cout << "Select Ticket Type:" << endl;
            cout << "1. VIP (Priority: 1) - Available: " << (maxVIPCapacity - currentVIPSeated) << "/" << maxVIPCapacity << endl;
            cout << "2. Influencer/Streamer (Priority: 2) - Available: " << (maxInfluencerCapacity - currentInfluencerSeated) << "/" << maxInfluencerCapacity << endl;
            cout << "3. General (Priority: 3) - Available: " << (maxGeneralCapacity - currentGeneralSeated) << "/" << maxGeneralCapacity << endl;
            cout << "Enter choice: ";
            cin >> ticketChoice;
        }
        
        // Check specific capacity limits before proceeding
        bool canProceed = false;
        
        switch (ticketChoice) {
            case 1:
                if (currentVIPSeated < maxVIPCapacity && currentPhysicalSeated < maxTotalPhysicalCapacity) {
                    newAudience.ticketType = "VIP";
                    newAudience.priority = 1;
                    newAudience.seatNumber = "VIP-" + to_string(currentVIPSeated + 1);
                    canProceed = true;
                } else {
                    cout << "VIP capacity full or physical capacity reached!" << endl;
                }
                break;
            case 2:
                if (currentInfluencerSeated < maxInfluencerCapacity && currentPhysicalSeated < maxTotalPhysicalCapacity) {
                    newAudience.ticketType = "Influencer";
                    newAudience.priority = 2;
                    newAudience.seatNumber = "INF-" + to_string(currentInfluencerSeated + 1);
                    canProceed = true;
                } else {
                    cout << "Influencer capacity full or physical capacity reached!" << endl;
                }
                break;
            case 3:
            default:
                if (viewingChoice == 2) {
                    // Online general
                    if (currentOnlineViewers < maxOnlineCapacity && currentGeneralSeated < maxGeneralCapacity) {
                        newAudience.ticketType = "Online";
                        newAudience.priority = 3;
                        newAudience.seatNumber = "ONL-" + to_string(currentOnlineViewers + 1);
                        canProceed = true;
                    } else {
                        cout << "Online or general capacity full!" << endl;
                    }
                } else {
                    // Physical general
                    int availableGeneralPhysical = maxTotalPhysicalCapacity - maxVIPCapacity - maxInfluencerCapacity;
                    int currentGeneralPhysical = currentPhysicalSeated - currentVIPSeated - currentInfluencerSeated;
                    
                    if (currentGeneralPhysical < availableGeneralPhysical && 
                        currentGeneralSeated < maxGeneralCapacity && 
                        currentPhysicalSeated < maxTotalPhysicalCapacity) {
                        newAudience.ticketType = "General";
                        newAudience.priority = 3;
                        newAudience.seatNumber = "GEN-" + to_string(currentGeneralSeated + 1);
                        canProceed = true;
                    } else {
                        cout << "General physical capacity full!" << endl;
                    }
                }
                break;
        }
        
        if (!canProceed) {
            cout << "Cannot proceed with ticket purchase due to capacity limits." << endl;
            return;
        }
        
        newAudience.isSeated = false;
        
        // Store audience member in array
        audience[audienceCount] = newAudience;
        audienceCount++;
        
        // Update counters
        currentSpectators++;
        totalSpectators++;
        
        if (newAudience.ticketType == "VIP") {
            totalVIPs++;
        } else if (newAudience.ticketType == "Influencer") {
            totalInfluencers++;
        } else {
            totalGeneral++;
        }
        
        // Create spectator info string for queues
        string spectatorInfo = newAudience.audienceID + " " + newAudience.name + 
                              " (" + newAudience.ticketType + ", " + viewingMode + ")";
        
        // Add to Priority Queue with proper priority
        spectatorPriorityQueue.enqueue(spectatorInfo, newAudience.priority);
        
        // Add to circular queue for overflow checking
        spectatorCircularQueue.enqueue(spectatorInfo);

        // Add to Circular Seating Queue for legacy compatibility
        seatingQueue.enqueue(newAudience.audienceID);
        
        cout << "\n=== TICKET PURCHASE & QUEUE ASSIGNMENT SUCCESSFUL ===" << endl;
        cout << "Audience ID: " << newAudience.audienceID << endl;
        cout << "Name: " << newAudience.name << endl;
        cout << "Ticket Type: " << newAudience.ticketType << " (Priority: " << newAudience.priority << ")" << endl;
        cout << "Viewing Mode: " << viewingMode << endl;
        cout << "Assigned Seat: " << newAudience.seatNumber << endl;
        cout << "Added to Priority Queue: Yes (Priority " << newAudience.priority << ")" << endl;
        cout << "Added to Seating Queue: Yes" << endl;
        
        // Save to file
        FileManager::saveAudience(audience, audienceCount);
        
        cout << "\nCurrent Capacity Status:" << endl;
        cout << "Total spectators: " << currentSpectators << "/" << maxTotalCapacity << endl;
        cout << "Physical seated: " << currentPhysicalSeated << "/" << maxTotalPhysicalCapacity << endl;
        cout << "VIPs: " << totalVIPs << "/" << maxVIPCapacity << ", Influencers: " << totalInfluencers << "/" << maxInfluencerCapacity << ", General: " << totalGeneral << "/" << maxGeneralCapacity << endl;
        cout << "Online viewers: " << currentOnlineViewers << "/" << maxOnlineCapacity << endl;
    }
    
    void seatAudienceMember() {
        string audienceID;
        cout << "\n=== SEAT AUDIENCE MEMBER ===" << endl;
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
        string seatType = audience[index].ticketType;
        
        // Check capacity based on ticket type
        if (seatType == "VIP") {
            if (currentVIPSeated < maxVIPCapacity && 
                currentPhysicalSeated < maxTotalPhysicalCapacity &&
                currentSpectators < maxTotalCapacity) {
                canSeat = true;
            }
        } else if (seatType == "Influencer" || seatType == "Streamer") {
            if (currentInfluencerSeated < maxInfluencerCapacity && 
                currentPhysicalSeated < maxTotalPhysicalCapacity &&
                currentSpectators < maxTotalCapacity) {
                canSeat = true;
            }
        } else if (seatType == "General") {
            if (currentGeneralSeated < maxGeneralCapacity && 
                currentPhysicalSeated < maxTotalPhysicalCapacity &&
                currentSpectators < maxTotalCapacity) {
                canSeat = true;
            }
        } else if (seatType == "Online") {
            if (currentOnlineViewers < maxOnlineCapacity && 
                currentGeneralSeated < maxGeneralCapacity &&
                currentSpectators < maxTotalCapacity) {
                canSeat = true;
            }
        }
        
        if (!canSeat) {
            cout << "Cannot seat audience member - capacity reached for " << seatType << " type!" << endl;
            viewCapacityStatus();
            return;
        }

        // Update counters
        audience[index].isSeated = true;
        updateSpectatorCounts(); // Recalculate all counts
        
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
        
        // Update and show current statistics
        updateSpectatorCounts();
        
        cout << "\nCurrent Statistics:" << endl;
        cout << "Total Registered: " << audienceCount << endl;
        cout << "Currently Seated: " << currentSpectators << "/" << maxTotalCapacity << endl;
        cout << "Physical Seated: " << currentPhysicalSeated << "/" << maxTotalPhysicalCapacity << endl;
        cout << "VIPs: " << currentVIPSeated << "/" << maxVIPCapacity << endl;
        cout << "Influencers: " << currentInfluencerSeated << "/" << maxInfluencerCapacity << endl;
        cout << "General: " << currentGeneralSeated << "/" << maxGeneralCapacity << endl;
        cout << "Online: " << currentOnlineViewers << "/" << maxOnlineCapacity << endl;
        cout << "In Queues - Total: " << totalSpectators << ", VIPs: " << totalVIPs << ", Influencers: " << totalInfluencers << ", General: " << totalGeneral << endl;
    }

    void processPrioritySeating() {
        cout << "\n=== PROCESSING PRIORITY SEATING (VIP -> Influencer -> General) ===" << endl;
        
        if (spectatorPriorityQueue.isEmpty()) {
            cout << "Priority queue is empty!" << endl;
            if (audienceCount > 0) {
                cout << "There are " << audienceCount << " registered audience members." << endl;
                cout << "Use option 8 to add them to the priority queue first." << endl;
            }
            return;
        }

        int seatedCount = 0;
        int processedCount = 0;
        
        // Update current counts before processing
        updateSpectatorCounts();
        
        while (!spectatorPriorityQueue.isEmpty()) {
            // Check total capacity
            if (currentSpectators >= maxTotalCapacity) {
                cout << "Maximum total capacity (" << maxTotalCapacity << ") reached. Stopping processing." << endl;
                break;
            }
            
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
                        currentSpectators++;
                        canSeat = true;
                        seatType = "VIP Physical";
                    } else if (isInfluencer && currentInfluencerSeated < maxInfluencerCapacity) {
                        currentInfluencerSeated++;
                        currentPhysicalSeated++;
                        currentSpectators++;
                        canSeat = true;
                        seatType = "Influencer Physical";
                    } else if (!isVIP && !isInfluencer && currentGeneralSeated < maxGeneralCapacity) {
                        // General audience - check available physical capacity for general
                        int availableGeneralPhysical = maxTotalPhysicalCapacity - maxVIPCapacity - maxInfluencerCapacity;
                        int currentGeneralPhysical = currentPhysicalSeated - currentVIPSeated - currentInfluencerSeated;
                        
                        if (currentGeneralPhysical < availableGeneralPhysical) {
                            currentGeneralSeated++;
                            currentPhysicalSeated++;
                            currentSpectators++;
                            canSeat = true;
                            seatType = "General Physical";
                        }
                    }
                }
            } else if (isOnline) {
                // Check online capacity
                if (currentOnlineViewers < maxOnlineCapacity && currentGeneralSeated < maxGeneralCapacity) {
                    currentOnlineViewers++;
                    currentGeneralSeated++;
                    currentSpectators++;
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
                string spectatorStr = spectatorInfo;
                overflowQueue.enqueue(spectatorStr);
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
        
        viewCapacityStatus();
    }
    
    void manageCircularOverflowQueue() {
        // Update counts first
        updateSpectatorCounts();
        
        // If audience exceeds capacity, fill overflowQueue from spectatorCircularQueue
        if (currentSpectators > maxTotalCapacity && overflowQueue.isEmpty()) {
            cout << "*** OVERFLOW MANAGEMENT REQUIRED ***" << endl;
            cout << "Total audiences (" << currentSpectators << ") exceed capacity limit of " << maxTotalCapacity << "." << endl;
            cout << "Populating overflow queue from circular spectator queue..." << endl;

            // Copy all spectators from circular queue to overflow queue
            CircularQueue temp = spectatorCircularQueue;
            totalOverflow = 0;

            int size = temp.size();
            for (int i = 0; i < size; i++) {
                string spectator = temp.dequeue();
                if (!overflowQueue.isFull()) {
                    overflowQueue.enqueue(spectator);
                    totalOverflow++;
                } else {
                    cout << "OverflowQueue is full. Could not add: " << spectator << endl;
                }
            }

            cout << "Overflow queue populated with " << totalOverflow << " spectators." << endl;
        }

        if (overflowQueue.isEmpty()) {
            cout << "No spectators in overflow queue." << endl;
            return;
        }

        bool managing = true;
        while (managing && !overflowQueue.isEmpty()) {
            cout << "\nSpectators in waiting list (Overflow Queue - FIFO order):" << endl;
            overflowQueue.display();
            cout << "Total in overflow: " << totalOverflow << endl;

            cout << "\nOverflow Management Options:" << endl;
            cout << "1. Process next overflow spectator" << endl;
            cout << "2. View overflow queue status" << endl;
            cout << "3. Clear overflow queue" << endl;
            cout << "4. Process all overflow spectators" << endl;
            cout << "5. Exit overflow management" << endl;
            cout << "Enter choice: ";

            int choice;
            cin >> choice;

            switch (choice) {
                case 1: {
                    if (!overflowQueue.isEmpty() && currentSpectators < maxTotalCapacity) {
                        string nextSpectator = overflowQueue.dequeue();
                        totalOverflow--;

                        bool seated = false;
                        if (currentPhysicalSeated < maxTotalPhysicalCapacity) {
                            currentPhysicalSeated++;
                            currentSpectators++;
                            seated = true;
                            cout << "Seated from overflow: " << nextSpectator << " (Physical)" << endl;
                        } else if (currentOnlineViewers < maxOnlineCapacity) {
                            currentOnlineViewers++;
                            currentSpectators++;
                            seated = true;
                            cout << "Seated from overflow: " << nextSpectator << " (Online)" << endl;
                        }

                        if (!seated) {
                            if (!overflowQueue.isFull()) {
                                overflowQueue.enqueue(nextSpectator);
                                totalOverflow++;
                            }
                            cout << "Cannot process: All capacities still full." << endl;
                        }
                    } else {
                        cout << "Cannot process: Queue empty or total capacity reached." << endl;
                    }
                    break;
                }

                case 2:
                    viewCapacityStatus();
                    break;

                case 3: {
                    int cleared = 0;
                    while (!overflowQueue.isEmpty()) {
                        overflowQueue.dequeue();
                        cleared++;
                    }
                    totalOverflow = 0;
                    cout << "Overflow queue cleared. Removed " << cleared << " spectators." << endl;
                    break;
                }

                case 4: {
                    int processed = 0;
                    while (!overflowQueue.isEmpty() && currentSpectators < maxTotalCapacity &&
                           (currentPhysicalSeated < maxTotalPhysicalCapacity ||
                            currentOnlineViewers < maxOnlineCapacity)) {

                        string nextSpectator = overflowQueue.dequeue();
                        totalOverflow--;
                        processed++;

                        if (currentPhysicalSeated < maxTotalPhysicalCapacity) {
                            currentPhysicalSeated++;
                            currentSpectators++;
                            cout << "Seated from overflow: " << nextSpectator << " (Physical)" << endl;
                        } else if (currentOnlineViewers < maxOnlineCapacity) {
                            currentOnlineViewers++;
                            currentSpectators++;
                            cout << "Seated from overflow: " << nextSpectator << " (Online)" << endl;
                        }
                    }
                    cout << "Processed " << processed << " spectators from overflow." << endl;
                    if (!overflowQueue.isEmpty()) {
                        cout << "Remaining in overflow: " << totalOverflow << endl;
                    }
                    break;
                }

                case 5:
                    managing = false;
                    break;

                default:
                    cout << "Invalid choice." << endl;
                    break;
            }
        }

        if (overflowQueue.isEmpty()) {
            cout << "Overflow queue is now empty. Management complete." << endl;
        }
    }
    
    // Modified removeSpectator() function
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
    
    if (!wasSeated) {
        cout << "Spectator " << spectatorId << " (" << type << ") was not seated. No capacity changes needed." << endl;
        return;
    }
    
    cout << "Processing spectator departure: " << spectatorId << " (" << type << ")" << endl;
    
    // Update capacity counters based on ticket type
    if (type == "VIP") {
        if (currentVIPSeated > 0) currentVIPSeated--;
        if (currentPhysicalSeated > 0) currentPhysicalSeated--;
        if (currentSpectators > 0) currentSpectators--;
        if (totalVIPs > 0) totalVIPs--;
        cout << "Removed VIP spectator from physical seating." << endl;
    } 
    else if (type == "Influencer" || type == "Streamer") {
        if (currentInfluencerSeated > 0) currentInfluencerSeated--;
        if (currentPhysicalSeated > 0) currentPhysicalSeated--;
        if (currentSpectators > 0) currentSpectators--;
        if (totalInfluencers > 0) totalInfluencers--;
        cout << "Removed Influencer spectator from physical seating." << endl;
    } 
    else if (type == "General") {
        if (currentGeneralSeated > 0) currentGeneralSeated--;
        if (currentPhysicalSeated > 0) currentPhysicalSeated--;
        if (currentSpectators > 0) currentSpectators--;
        if (totalGeneral > 0) totalGeneral--;
        cout << "Removed General spectator from physical seating." << endl;
    } 
    else if (type == "Online") {
        if (currentOnlineViewers > 0) currentOnlineViewers--;
        if (currentGeneralSeated > 0) currentGeneralSeated--;
        if (currentSpectators > 0) currentSpectators--;
        if (totalOnline > 0) totalOnline--;
        cout << "Removed Online viewer." << endl;
    }
    
    // Update total spectators count
    if (totalSpectators > 0) totalSpectators--;
    
    // Mark as not seated
    audience[index].isSeated = false;
    
    cout << "Spectator " << spectatorId << " (" << type << ") removed successfully." << endl;
    
    // Try to seat someone from overflow queue if there's available capacity
    if (!overflowQueue.isEmpty() && currentSpectators < maxTotalCapacity) {
        string nextSpectator = overflowQueue.dequeue();
        totalOverflow--;
        
        // Determine what type of seating is available
        bool seated = false;
        
        // Check for VIP capacity
        if (nextSpectator.find("(VIP,") != string::npos && 
            currentVIPSeated < maxVIPCapacity && 
            currentPhysicalSeated < maxTotalPhysicalCapacity) {
            currentVIPSeated++;
            currentPhysicalSeated++;
            currentSpectators++;
            totalVIPs++;
            totalSpectators++;
            seated = true;
            cout << "Seated VIP from overflow queue: " << nextSpectator << endl;
        }
        // Check for Influencer capacity
        else if (nextSpectator.find("(Influencer,") != string::npos && 
                 currentInfluencerSeated < maxInfluencerCapacity && 
                 currentPhysicalSeated < maxTotalPhysicalCapacity) {
            currentInfluencerSeated++;
            currentPhysicalSeated++;
            currentSpectators++;
            totalInfluencers++;
            totalSpectators++;
            seated = true;
            cout << "Seated Influencer from overflow queue: " << nextSpectator << endl;
        }
        // Check for General physical capacity
        else if (nextSpectator.find("Physical)") != string::npos && 
                 currentGeneralSeated < maxGeneralCapacity && 
                 currentPhysicalSeated < maxTotalPhysicalCapacity) {
            currentGeneralSeated++;
            currentPhysicalSeated++;
            currentSpectators++;
            totalGeneral++;
            totalSpectators++;
            seated = true;
            cout << "Seated General (Physical) from overflow queue: " << nextSpectator << endl;
        }
        // Check for Online capacity
        else if (nextSpectator.find("Online)") != string::npos && 
                 currentOnlineViewers < maxOnlineCapacity && 
                 currentGeneralSeated < maxGeneralCapacity) {
            currentOnlineViewers++;
            currentGeneralSeated++;
            currentSpectators++;
            totalOnline++;
            totalSpectators++;
            seated = true;
            cout << "Seated Online viewer from overflow queue: " << nextSpectator << endl;
        }
        
        if (!seated) {
            // Put back in overflow if no capacity available
            overflowQueue.enqueue(nextSpectator);
            totalOverflow++;
            cout << "Could not seat from overflow - no available capacity for spectator type." << endl;
        }
    }
    
    // Display updated capacity status
    cout << "\nUpdated Capacity Status:" << endl;
    cout << "Total Spectators: " << currentSpectators << "/" << maxTotalCapacity << endl;
    cout << "Physical Seated: " << currentPhysicalSeated << "/" << maxTotalPhysicalCapacity << endl;
    cout << "VIP: " << currentVIPSeated << "/" << maxVIPCapacity << ", ";
    cout << "Influencer: " << currentInfluencerSeated << "/" << maxInfluencerCapacity << ", ";
    cout << "General: " << currentGeneralSeated << "/" << maxGeneralCapacity << ", ";
    cout << "Online: " << currentOnlineViewers << "/" << maxOnlineCapacity << endl;
    
    FileManager::saveAudience(audience, audienceCount);
}

// Modified addAllAudienceToQueue() function
void addAllAudienceToQueue() {
    cout << "\n=== ADD ALL EXISTING AUDIENCE TO PRIORITY & CIRCULAR QUEUES ===" << endl;

    // First, ensure audience data is loaded from CSV
    if (audienceCount == 0) {
        cout << "No audience data found. Attempting to load from CSV..." << endl;
        FileManager::loadAudience(audience, audienceCount);
        
        if (audienceCount == 0) {
            cout << "No audience members found in CSV file or database." << endl;
            return;
        }
    }

    int addedCount = 0;
    cout << "Processing " << audienceCount << " audience members from database..." << endl;

    // Reset queue statistics (not seated statistics)
    totalSpectators = 0;
    totalVIPs = 0;
    totalInfluencers = 0;
    totalGeneral = 0;
    totalOnline = 0;

    // Clear existing queues to avoid duplicates
    cout << "Clearing existing queues..." << endl;
    
    // Clear priority queue
    while (!spectatorPriorityQueue.isEmpty()) {
        spectatorPriorityQueue.dequeue();
    }
    
    // Clear circular queue
    while (!spectatorCircularQueue.isEmpty()) {
        spectatorCircularQueue.dequeue();
    }

    // Update seated counts from audience array
    updateSpectatorCounts();

    for (int i = 0; i < audienceCount; i++) {
        if (!audience[i].isSeated) {  // Only add unseated members
            // Convert ticket type to spectator type for display
            string spectatorType = audience[i].ticketType;
            if (spectatorType == "Streamer") {
                spectatorType = "Influencer";
            }

            // Determine viewing mode based on ticket type
            string viewingMode;
            if (audience[i].ticketType == "Online") {
                viewingMode = "Online";
            } else {
                viewingMode = "Physical";
            }

            // Create spectator info string
            string spectatorInfo = audience[i].audienceID + " " + audience[i].name +
                                  " (" + spectatorType + ", " + viewingMode + ")";

            // Check queue capacity before adding
            if (!spectatorCircularQueue.isFull()) {
                // Enqueue into priority queue (uses priority from CSV)
                spectatorPriorityQueue.enqueue(spectatorInfo, audience[i].priority);
                
                // Enqueue into circular queue (FIFO order)
                spectatorCircularQueue.enqueue(spectatorInfo);

                // Update queue statistics based on spectator type
                totalSpectators++;
                if (spectatorType == "VIP") {
                    totalVIPs++;
                } else if (spectatorType == "Influencer") {
                    totalInfluencers++;
                } else if (audience[i].ticketType == "Online") {
                    totalOnline++;
                    totalGeneral++; // Online viewers are part of general category
                } else {
                    totalGeneral++;
                }

                addedCount++;

                cout << "Added to queues: " << spectatorInfo 
                     << " (Priority: " << audience[i].priority 
                     << ", Seat: " << (audience[i].seatNumber.empty() || audience[i].seatNumber == "-" ? "N/A" : audience[i].seatNumber) 
                     << ")" << endl;
            } else {
                cout << "Warning: Circular queue is full. Cannot add more members." << endl;
                break;
            }
        } else {
            cout << "Skipping already seated member: " << audience[i].audienceID 
                 << " " << audience[i].name << " (Seat: " << audience[i].seatNumber << ")" << endl;
        }
    }

    cout << "\n=== QUEUE POPULATION COMPLETE ===" << endl;
    cout << "Successfully added " << addedCount << " audience members to both queues." << endl;
    cout << "Total in queues: " << totalSpectators << endl;
    cout << "Queue breakdown - VIPs: " << totalVIPs 
         << ", Influencers: " << totalInfluencers 
         << ", General: " << (totalGeneral - totalOnline)
         << ", Online: " << totalOnline << endl;

    // Display current seated vs queue statistics
    cout << "\n=== CURRENT STATISTICS SUMMARY ===" << endl;
    cout << "SEATED SPECTATORS:" << endl;
    cout << "  Total Seated: " << currentSpectators << "/" << maxTotalCapacity << endl;
    cout << "  Physical Seated: " << currentPhysicalSeated << "/" << maxTotalPhysicalCapacity << endl;
    cout << "  VIP Seated: " << currentVIPSeated << "/" << maxVIPCapacity << endl;
    cout << "  Influencer Seated: " << currentInfluencerSeated << "/" << maxInfluencerCapacity << endl;
    cout << "  General Seated: " << currentGeneralSeated << "/" << maxGeneralCapacity << endl;
    cout << "  Online Viewers: " << currentOnlineViewers << "/" << maxOnlineCapacity << endl;
    cout << "\nIN QUEUES (WAITING):" << endl;
    cout << "  Total in Queues: " << totalSpectators << endl;
    cout << "  VIPs: " << totalVIPs << ", Influencers: " << totalInfluencers 
         << ", General: " << (totalGeneral - totalOnline) << ", Online: " << totalOnline << endl;

    if (addedCount == 0) {
        if (audienceCount > 0) {
            cout << "All " << audienceCount << " audience members are already seated." << endl;
        } else {
            cout << "No audience data available to add to queues." << endl;
        }
    }

    // Display current queue states
    cout << "\n=== CURRENT QUEUE STATES ===" << endl;
    cout << "\nPriority Queue (ordered by priority):" << endl;
    spectatorPriorityQueue.display();
    
    cout << "\nCircular Queue (FIFO order):" << endl;
    spectatorCircularQueue.display();
}

// Modified viewCapacityStatus() function
void viewCapacityStatus() {
    // Update all counts before displaying
    updateSpectatorCounts();
    
    cout << "\n" << string(70, '=') << endl;
    cout << "    COMPREHENSIVE CAPACITY STATUS REPORT" << endl;
    cout << string(70, '=') << endl;
    
    // Calculate percentages
    double totalCapacityPercent = (maxTotalCapacity > 0) ? (double)currentSpectators/maxTotalCapacity*100 : 0;
    double physicalPercent = (maxTotalPhysicalCapacity > 0) ? (double)currentPhysicalSeated/maxTotalPhysicalCapacity*100 : 0;
    double vipPercent = (maxVIPCapacity > 0) ? (double)currentVIPSeated/maxVIPCapacity*100 : 0;
    double influencerPercent = (maxInfluencerCapacity > 0) ? (double)currentInfluencerSeated/maxInfluencerCapacity*100 : 0;
    double generalPercent = (maxGeneralCapacity > 0) ? (double)currentGeneralSeated/maxGeneralCapacity*100 : 0;
    double onlinePercent = (maxOnlineCapacity > 0) ? (double)currentOnlineViewers/maxOnlineCapacity*100 : 0;
    
    cout << "TOTAL CAPACITY (ALL SPECTATORS):" << endl;
    cout << "  Total Spectators: " << currentSpectators << "/" << maxTotalCapacity 
         << " (" << fixed << setprecision(1) << totalCapacityPercent << "%)" << endl;
    
    cout << "\nPHYSICAL SEATING BREAKDOWN:" << endl;
    cout << "  Total Physical: " << currentPhysicalSeated << "/" << maxTotalPhysicalCapacity 
         << " (" << physicalPercent << "%)" << endl;
    cout << "  VIP Seats: " << currentVIPSeated << "/" << maxVIPCapacity 
         << " (" << vipPercent << "%)" << endl;
    cout << "  Influencer Seats: " << currentInfluencerSeated << "/" << maxInfluencerCapacity 
         << " (" << influencerPercent << "%)" << endl;
    
    // Calculate general physical capacity
    int generalPhysicalSeated = currentPhysicalSeated - currentVIPSeated - currentInfluencerSeated;
    int maxGeneralPhysical = maxTotalPhysicalCapacity - maxVIPCapacity - maxInfluencerCapacity; // 150 max
    double generalPhysicalPercent = (maxGeneralPhysical > 0) ? (double)generalPhysicalSeated/maxGeneralPhysical*100 : 0;
    
    cout << "  General Physical: " << generalPhysicalSeated << "/" << maxGeneralPhysical 
         << " (" << generalPhysicalPercent << "%)" << endl;
    
    cout << "\nONLINE VIEWING:" << endl;
    cout << "  Online Viewers: " << currentOnlineViewers << "/" << maxOnlineCapacity 
         << " (" << onlinePercent << "%)" << endl;
    
    cout << "\nCATEGORY TOTALS:" << endl;
    cout << "  VIP Total: " << currentVIPSeated << "/" << maxVIPCapacity 
         << " (" << vipPercent << "%)" << endl;
    cout << "  Influencer Total: " << currentInfluencerSeated << "/" << maxInfluencerCapacity 
         << " (" << influencerPercent << "%)" << endl;
    cout << "  General Total: " << currentGeneralSeated << "/" << maxGeneralCapacity 
         << " (" << generalPercent << "%) [Physical: " << generalPhysicalSeated 
         << ", Online: " << currentOnlineViewers << "]" << endl;
    
    cout << "\nQUEUE STATUS:" << endl;
    cout << "  Spectators in Priority Queue: " << totalSpectators << endl;
    cout << "  VIPs Waiting: " << totalVIPs << endl;
    cout << "  Influencers Waiting: " << totalInfluencers << endl;
    cout << "  General Waiting: " << (totalGeneral - totalOnline) << endl;
    cout << "  Online Waiting: " << totalOnline << endl;
    cout << "  Waiting in Overflow: " << totalOverflow << endl;
    
    cout << "\nREGISTRATION STATUS:" << endl;
    cout << "  Total Registered: " << audienceCount << endl;
    cout << "  Currently Seated: " << currentSpectators << endl;
    cout << "  In Queues: " << totalSpectators << endl;
    cout << "  In Overflow: " << totalOverflow << endl;
    
    // Capacity warnings
    cout << "\nCAPACITY ALERTS:" << endl;
    if (currentSpectators >= maxTotalCapacity) {
        cout << "TOTAL CAPACITY FULL - No new spectators can be seated!" << endl;
    } else if (totalCapacityPercent >= 90) {
        cout << "TOTAL CAPACITY CRITICAL - " << (maxTotalCapacity - currentSpectators) << " seats remaining" << endl;
    }
    
    if (currentPhysicalSeated >= maxTotalPhysicalCapacity) {
        cout << "PHYSICAL CAPACITY FULL - Only online viewing available!" << endl;
    } else if (physicalPercent >= 90) {
        cout << "PHYSICAL CAPACITY CRITICAL - " << (maxTotalPhysicalCapacity - currentPhysicalSeated) << " physical seats remaining" << endl;
    }
    
    if (currentOnlineViewers >= maxOnlineCapacity) {
        cout << "ONLINE CAPACITY FULL!" << endl;
    } else if (onlinePercent >= 90) {
        cout << "ONLINE CAPACITY CRITICAL - " << (maxOnlineCapacity - currentOnlineViewers) << " online slots remaining" << endl;
    }
    
    cout << string(70, '=') << endl;
}

public:
    SpectatorManager() : spectatorCircularQueue(50), seatingQueue(50), overflowQueue(50) {
        // Initialize audience array
        audienceCount = 0;
        
        // Initialize ENHANCED CAPACITY MANAGEMENT - All variables properly defined
        maxTotalCapacity = 300;              // 300 total spectators (VIP + Influencer + General)
        maxTotalPhysicalCapacity = 200;      // 200 total physical seats
        maxVIPCapacity = 20;                 // 20 VIP physical seats
        maxInfluencerCapacity = 30;          // 30 Influencer physical seats
        maxGeneralCapacity = 150;            // 150 general capacity (physical + online)
        maxOnlineCapacity = 100;             // 100 online capacity
        
        // Initialize ENHANCED CURRENT COUNTS - All variables properly defined
        currentSpectators = 0;               // Current total spectators (all types)
        currentPhysicalSeated = 0;           // Current physical attendees
        currentVIPSeated = 0;                // Current VIP seated
        currentInfluencerSeated = 0;         // Current Influencer seated
        currentGeneralSeated = 0;            // Current General seated (physical + online)
        currentOnlineViewers = 0;            // Current online viewers
        
        // Initialize ENHANCED STATISTICS TRACKING - All variables properly defined
        totalSpectators = 0;                 // Total in queues/registered
        totalPhysicalSpectators = 0;         // Total physical spectators
        totalVIPs = 0;                       // Total VIPs in system
        totalInfluencers = 0;                // Total Influencers in system
        totalGeneral = 0;                    // Total General in system
        totalOnline = 0;                     // Total Online viewers in system
        totalOverflow = 0;                   // Total in overflow queue
        
        // Load existing audience data from file
        FileManager::loadAudience(audience, audienceCount);
    }

    void runAudience() {
        int choice;
        
        do {
            displayAudienceMenu();
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
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    break;
            }
            
            if (choice != 0) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
            
        } while (choice != 0);
    }

    void runAdmin() {
        int choice;
        
        do {
            displayAdminMenu();
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            
            switch (choice) {
                case 1:
                    seatAudienceMember();
                    break;
                case 2:
                    viewAllAudience();
                    break;
                case 3:
                    processPrioritySeating();
                    break;
                case 4:
                    manageCircularOverflowQueue();
                    break;
                case 5:
                    removeSpectator();
                    break;
                case 6:
                    viewCapacityStatus();
                    break;
                case 7:
                    addAllAudienceToQueue();
                    break;
                case 0:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    break;
            }
            
            if (choice != 0) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
            
        } while (choice != 0);
    }
};