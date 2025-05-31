#include <iostream>
#include <string>
#include <iomanip>
#include "structs.h"
#include "helper.h"
#include "CircularQueue.h"
#include "PriorityQueue.h"

using namespace std;

class SpectatorManager {
private:
    Audience audience[100];
    int audienceCount;
    CircularQueue seatingQueue;
    PriorityQueue viewingPriorityQueue;
    int maxCapacity;
    int currentSeated;
    
    void displayMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    LIVE STREAM & SPECTATOR QUEUE MANAGEMENT" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Buy Ticket & Join Audience" << endl;
        cout << "2. Seat Audience Member" << endl;
        cout << "3. View Seating Queue" << endl;
        cout << "4. View Priority Queue (VIP/Streamers)" << endl;
        cout << "5. Process Next Spectator" << endl;
        cout << "6. View All Audience Members" << endl;
        cout << "7. Manage Overflow Audience" << endl;
        cout << "8. Setup Live Stream Slots" << endl;
        cout << "9. Display Seating Statistics" << endl;
        cout << "10. Remove Audience Member" << endl;
        cout << "11. Back to Main Menu" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter your choice: ";
    }
    
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
        
        cout << "Select Ticket Type:" << endl;
        cout << "1. VIP (Priority: 1)" << endl;
        cout << "2. Streamer (Priority: 2)" << endl;
        cout << "3. General (Priority: 3)" << endl;
        cout << "Enter choice: ";
        
        int ticketChoice;
        cin >> ticketChoice;
        
        switch (ticketChoice) {
            case 1:
                newAudience.ticketType = "VIP";
                newAudience.priority = 1;
                newAudience.seatNumber = "VIP-" + to_string(audienceCount + 1);
                break;
            case 2:
                newAudience.ticketType = "Streamer";
                newAudience.priority = 2;
                newAudience.seatNumber = "STR-" + to_string(audienceCount + 1);
                break;
            case 3:
                newAudience.ticketType = "General";
                newAudience.priority = 3;
                newAudience.seatNumber = "GEN-" + to_string(audienceCount + 1);
                break;
            default:
                cout << "Invalid choice! Setting as General ticket." << endl;
                newAudience.ticketType = "General";
                newAudience.priority = 3;
                newAudience.seatNumber = "GEN-" + to_string(audienceCount + 1);
                break;
        }
        
        newAudience.isSeated = false;
        
        audience[audienceCount] = newAudience;
        audienceCount++;
        
        // Add to appropriate queue
        if (newAudience.priority <= 2) { // VIP or Streamer
            viewingPriorityQueue.enqueue(newAudience.audienceID + " - " + newAudience.name + " (" + newAudience.ticketType + ")", newAudience.priority);
        }
        
        // Add to circular seating queue for seat assignment
        seatingQueue.enqueue(newAudience.audienceID[3]); // Use last character of ID
        
        cout << "Ticket purchased successfully!" << endl;
        cout << "Audience ID: " << newAudience.audienceID << endl;
        cout << "Ticket Type: " << newAudience.ticketType << endl;
        cout << "Assigned Seat: " << newAudience.seatNumber << endl;
        
        FileManager::saveAudience(audience, audienceCount);
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
        
        if (currentSeated >= maxCapacity) {
            cout << "Venue is at full capacity! Cannot seat more audience members." << endl;
            return;
        }
        
        audience[index].isSeated = true;
        currentSeated++;
        
        cout << "Audience member " << audience[index].name << " seated successfully!" << endl;
        cout << "Seat Number: " << audience[index].seatNumber << endl;
        cout << "Ticket Type: " << audience[index].ticketType << endl;
        cout << "Seating time: " << Utils::getCurrentTimestamp() << endl;
        
        FileManager::saveAudience(audience, audienceCount);
    }
    
    void viewSeatingQueue() {
        cout << "\n=== SEATING QUEUE STATUS ===" << endl;
        
        if (seatingQueue.isEmpty()) {
            cout << "No audience members in seating queue." << endl;
        } else {
            cout << "Current seating queue:" << endl;
            seatingQueue.display();
        }
        
        cout << "Queue Size: " << seatingQueue.size() << endl;
        cout << "Current Seated: " << currentSeated << "/" << maxCapacity << endl;
    }
    
    void viewPriorityQueue() {
        cout << "\n=== PRIORITY VIEWING QUEUE (VIP/STREAMERS) ===" << endl;
        
        if (viewingPriorityQueue.isEmpty()) {
            cout << "No VIP or Streamers in priority queue." << endl;
        } else {
            viewingPriorityQueue.display();
        }
    }
    
    void processNextSpectator() {
        cout << "\n=== PROCESSING NEXT SPECTATOR ===" << endl;
        
        // Process priority queue first (VIP/Streamers)
        if (!viewingPriorityQueue.isEmpty()) {
            string nextSpectator = viewingPriorityQueue.dequeue();
            cout << "Processing from Priority Queue: " << nextSpectator << endl;
            cout << "Special viewing arrangement provided!" << endl;
        }
        // Then process circular seating queue
        else if (!seatingQueue.isEmpty()) {
            char nextSpectator = seatingQueue.dequeue();
            cout << "Processing from Seating Queue: Audience with ID ending in '" << nextSpectator << "'" << endl;
            cout << "Spectator directed to seating!" << endl;
        }
        else {
            cout << "No spectators in queue to process!" << endl;
        }
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
    }
    
    void manageOverflowAudience() {
        cout << "\n=== OVERFLOW AUDIENCE MANAGEMENT ===" << endl;
        
        int unseatedCount = 0;
        for (int i = 0; i < audienceCount; i++) {
            if (!audience[i].isSeated) {
                unseatedCount++;
            }
        }
        
        cout << "Current Capacity: " << currentSeated << "/" << maxCapacity << endl;
        cout << "Unseated Audience: " << unseatedCount << endl;
        
        if (unseatedCount > 0) {
            cout << "\nOverflow Management Options:" << endl;
            cout << "1. Increase venue capacity" << endl;
            cout << "2. Setup overflow viewing area" << endl;
            cout << "3. Offer live stream viewing" << endl;
            cout << "Enter choice: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1:
                    cout << "Enter additional capacity: ";
                    int additional;
                    cin >> additional;
                    maxCapacity += additional;
                    cout << "Venue capacity increased to " << maxCapacity << endl;
                    break;
                case 2:
                    cout << "Overflow viewing area setup for " << unseatedCount << " people." << endl;
                    break;
                case 3:
                    cout << "Live stream viewing arranged for overflow audience." << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
                    break;
            }
        } else {
            cout << "No overflow audience to manage." << endl;
        }
    }
    
    void setupLiveStreamSlots() {
        cout << "\n=== LIVE STREAM SLOTS SETUP ===" << endl;
        
        int streamerCount = 0;
        for (int i = 0; i < audienceCount; i++) {
            if (audience[i].ticketType == "Streamer") {
                streamerCount++;
            }
        }
        
        cout << "Number of Streamers: " << streamerCount << endl;
        
        if (streamerCount > 0) {
            cout << "Setting up dedicated streaming slots..." << endl;
            cout << "Streaming Equipment Check:" << endl;
            cout << "- Camera positions: " << streamerCount << " slots" << endl;
            cout << "- Audio equipment: Ready" << endl;
            cout << "- Network bandwidth: Allocated" << endl;
            cout << "- Streaming software: Configured" << endl;
            
            cout << "\nLive stream slots ready for " << streamerCount << " streamers!" << endl;
        } else {
            cout << "No streamers registered yet." << endl;
        }
    }
    
    void displaySeatingStats() {
        cout << "\n=== SEATING STATISTICS ===" << endl;
        cout << "Total Audience Members: " << audienceCount << endl;
        cout << "Currently Seated: " << currentSeated << endl;
        cout << "Venue Capacity: " << maxCapacity << endl;
        cout << "Available Seats: " << (maxCapacity - currentSeated) << endl;
        cout << "Seating Queue Size: " << seatingQueue.size() << endl;
        cout << "Priority Queue Size: " << viewingPriorityQueue.size() << endl;
        
        int vipCount = 0, streamerCount = 0, generalCount = 0;
        int seatedVip = 0, seatedStreamer = 0, seatedGeneral = 0;
        
        for (int i = 0; i < audienceCount; i++) {
            if (audience[i].ticketType == "VIP") {
                vipCount++;
                if (audience[i].isSeated) seatedVip++;
            } else if (audience[i].ticketType == "Streamer") {
                streamerCount++;
                if (audience[i].isSeated) seatedStreamer++;
            } else {
                generalCount++;
                if (audience[i].isSeated) seatedGeneral++;
            }
        }
        
        cout << "\nTicket Type Breakdown:" << endl;
        cout << "VIP: " << vipCount << " (Seated: " << seatedVip << ")" << endl;
        cout << "Streamer: " << streamerCount << " (Seated: " << seatedStreamer << ")" << endl;
        cout << "General: " << generalCount << " (Seated: " << seatedGeneral << ")" << endl;
        
        if (maxCapacity > 0) {
            double occupancyRate = (double(currentSeated) / maxCapacity) * 100;
            cout << "Occupancy Rate: " << fixed << setprecision(1) << occupancyRate << "%" << endl;
        }
    }
    
    void removeAudienceMember() {
        string audienceID;
        cout << "\n=== REMOVE AUDIENCE MEMBER ===" << endl;
        cout << "Enter Audience ID to remove: ";
        cin >> audienceID;
        
        int index = Utils::findAudienceIndex(audience, audienceCount, audienceID);
        if (index == -1) {
            cout << "Audience member not found!" << endl;
            return;
        }
        
        cout << "Removing audience member: " << audience[index].name << endl;
        
        if (audience[index].isSeated) {
            currentSeated--;
        }
        
        // Shift audience array to remove the member
        for (int i = index; i < audienceCount - 1; i++) {
            audience[i] = audience[i + 1];
        }
        audienceCount--;
        
        cout << "Audience member removed successfully!" << endl;
        
        FileManager::saveAudience(audience, audienceCount);
    }

public:
    SpectatorManager() : seatingQueue(50) { // Circular queue with capacity of 50
        audienceCount = 0;
        maxCapacity = 100;
        currentSeated = 0;
        FileManager::loadAudience(audience, audienceCount);
        
        // Count already seated audience members
        for (int i = 0; i < audienceCount; i++) {
            if (audience[i].isSeated) {
                currentSeated++;
            }
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
                    viewSeatingQueue();
                    break;
                case 4:
                    viewPriorityQueue();
                    break;
                case 5:
                    processNextSpectator();
                    break;
                case 6:
                    viewAllAudience();
                    break;
                case 7:
                    manageOverflowAudience();
                    break;
                case 8:
                    setupLiveStreamSlots();
                    break;
                case 9:
                    displaySeatingStats();
                    break;
                case 10:
                    removeAudienceMember();
                    break;
                case 11:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid option! Please try again." << endl;
                    break;
            }
            
            if (choice != 11) {
                Utils::pauseScreen();
            }
            
        } while (choice != 11);
    }
};