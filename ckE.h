#include <iostream>
#include <string>
#include <iomanip>
#include "structs.h"
#include "helper.h"
#include "Queue.h"
#include "PriorityQueue.h"

using namespace std;

class RegistrationManager {
private:
    Player players[100];
    int playerCount;
    Queue normalQueue;
    PriorityQueue priorityQueue;
    
    void displayMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    TOURNAMENT REGISTRATION & PLAYER QUEUEING" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Register New Player" << endl;
        cout << "2. Check-In Player" << endl;
        cout << "3. View Registration Queue" << endl;
        cout << "4. Process Next Player in Queue" << endl;
        cout << "5. View All Registered Players" << endl;
        cout << "6. Add Wildcard Entry" << endl;
        cout << "7. Handle Player Withdrawal" << endl;
        cout << "8. Display Queue Statistics" << endl;
        cout << "9. Back to Main Menu" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter your choice: ";
    }
    
    void registerNewPlayer() {
        if (playerCount >= 100) {
            cout << "Maximum player capacity reached!" << endl;
            return;
        }
        
        Player newPlayer;
        cout << "\n=== PLAYER REGISTRATION ===" << endl;
        
        // Generate new player ID
        newPlayer.playerID = Utils::generatePlayerID(playerCount);
        
        cout << "Player ID: " << newPlayer.playerID << endl;
        cout << "Enter player name: ";
        cin.ignore();
        getline(cin, newPlayer.name);
        
        cout << "Enter email: ";
        getline(cin, newPlayer.email);
        
        cout << "Enter university: ";
        getline(cin, newPlayer.university);
        
        cout << "Enter ranking (1000-2000): ";
        cin >> newPlayer.ranking;
        
        newPlayer.registrationDate = Utils::getCurrentDate();
        
        char earlyBird;
        cout << "Is this an early-bird registration? (y/n): ";
        cin >> earlyBird;
        newPlayer.isEarlyBird = (earlyBird == 'y' || earlyBird == 'Y');
        
        newPlayer.isWildcard = false;
        newPlayer.isCheckedIn = false;
        newPlayer.wins = 0;
        newPlayer.losses = 0;
        
        players[playerCount] = newPlayer;
        playerCount++;
        
        // Add to appropriate queue
        if (newPlayer.isEarlyBird) {
            priorityQueue.enqueue(newPlayer.playerID + " - " + newPlayer.name, 1); // Higher priority for early birds
        } else {
            normalQueue.addQueue(newPlayer.playerID[3]); // Use last character of ID
        }
        
        cout << "Player registered successfully!" << endl;
        cout << "Registration Date: " << newPlayer.registrationDate << endl;
        
        // Save to file
        FileManager::savePlayers(players, playerCount);
    }
    
    void checkInPlayer() {
        string playerID;
        cout << "\n=== PLAYER CHECK-IN ===" << endl;
        cout << "Enter Player ID: ";
        cin >> playerID;
        
        int index = Utils::findPlayerIndex(players, playerCount, playerID);
        if (index == -1) {
            cout << "Player not found!" << endl;
            return;
        }
        
        if (players[index].isCheckedIn) {
            cout << "Player " << players[index].name << " is already checked in!" << endl;
            return;
        }
        
        players[index].isCheckedIn = true;
        cout << "Player " << players[index].name << " checked in successfully!" << endl;
        cout << "Check-in time: " << Utils::getCurrentTimestamp() << endl;
        
        FileManager::savePlayers(players, playerCount);
    }
    
    void viewRegistrationQueue() {
        cout << "\n=== REGISTRATION QUEUE STATUS ===" << endl;
        
        cout << "\nPRIORITY QUEUE (Early Birds & Wildcards):" << endl;
        if (priorityQueue.isEmpty()) {
            cout << "No players in priority queue." << endl;
        } else {
            priorityQueue.display();
        }
        
        cout << "\nNORMAL QUEUE (Regular Registration):" << endl;
        if (normalQueue.isEmpty()) {
            cout << "No players in normal queue." << endl;
        } else {
            normalQueue.display();
        }
    }
    
    void processNextPlayer() {
        cout << "\n=== PROCESSING NEXT PLAYER ===" << endl;
        
        // Process priority queue first
        if (!priorityQueue.isEmpty()) {
            string nextPlayer = priorityQueue.dequeue();
            cout << "Processing from Priority Queue: " << nextPlayer << endl;
            cout << "Player processed and ready for tournament!" << endl;
        }
        else if (!normalQueue.isEmpty()) {
            char nextPlayer = normalQueue.deleteQueue();
            cout << "Processing from Normal Queue: Player with ID ending in '" << nextPlayer << "'" << endl;
            cout << "Player processed and ready for tournament!" << endl;
        }
        else {
            cout << "No players in queue to process!" << endl;
        }
    }
    
    void viewAllPlayers() {
        cout << "\n=== ALL REGISTERED PLAYERS ===" << endl;
        
        if (playerCount == 0) {
            cout << "No players registered yet." << endl;
            return;
        }
        
        cout << left << setw(8) << "ID" 
             << setw(20) << "Name" 
             << setw(25) << "University" 
             << setw(8) << "Ranking" 
             << setw(12) << "Early Bird" 
             << setw(10) << "Wildcard" 
             << setw(10) << "Checked In" << endl;
        cout << string(93, '-') << endl;
        
        for (int i = 0; i < playerCount; i++) {
            cout << left << setw(8) << players[i].playerID
                 << setw(20) << players[i].name
                 << setw(25) << players[i].university
                 << setw(8) << players[i].ranking
                 << setw(12) << (players[i].isEarlyBird ? "Yes" : "No")
                 << setw(10) << (players[i].isWildcard ? "Yes" : "No")
                 << setw(10) << (players[i].isCheckedIn ? "Yes" : "No") << endl;
        }
    }
    
    void addWildcardEntry() {
        if (playerCount >= 100) {
            cout << "Maximum player capacity reached!" << endl;
            return;
        }
        
        Player wildcard;
        cout << "\n=== WILDCARD ENTRY ===" << endl;
        
        wildcard.playerID = Utils::generatePlayerID(playerCount);
        
        cout << "Wildcard Player ID: " << wildcard.playerID << endl;
        cout << "Enter player name: ";
        cin.ignore();
        getline(cin, wildcard.name);
        
        cout << "Enter email: ";
        getline(cin, wildcard.email);
        
        cout << "Enter university: ";
        getline(cin, wildcard.university);
        
        cout << "Enter ranking (1000-2000): ";
        cin >> wildcard.ranking;
        
        wildcard.registrationDate = Utils::getCurrentDate();
        wildcard.isEarlyBird = false;
        wildcard.isWildcard = true;
        wildcard.isCheckedIn = false;
        wildcard.wins = 0;
        wildcard.losses = 0;
        
        players[playerCount] = wildcard;
        playerCount++;
        
        // Add wildcard to priority queue with high priority
        priorityQueue.enqueue(wildcard.playerID + " - " + wildcard.name + " (WILDCARD)", 2);
        
        cout << "Wildcard player registered successfully!" << endl;
        cout << "Registration Date: " << wildcard.registrationDate << endl;
        
        FileManager::savePlayers(players, playerCount);
    }
    
    void handleWithdrawal() {
        string playerID;
        cout << "\n=== PLAYER WITHDRAWAL ===" << endl;
        cout << "Enter Player ID to withdraw: ";
        cin >> playerID;
        
        int index = Utils::findPlayerIndex(players, playerCount, playerID);
        if (index == -1) {
            cout << "Player not found!" << endl;
            return;
        }
        
        cout << "Withdrawing player: " << players[index].name << endl;
        
        // Shift players array to remove the withdrawn player
        for (int i = index; i < playerCount - 1; i++) {
            players[i] = players[i + 1];
        }
        playerCount--;
        
        cout << "Player withdrawn successfully!" << endl;
        cout << "Withdrawal processed at: " << Utils::getCurrentTimestamp() << endl;
        
        FileManager::savePlayers(players, playerCount);
    }
    
    void displayQueueStats() {
        cout << "\n=== QUEUE STATISTICS ===" << endl;
        cout << "Priority Queue Size: " << priorityQueue.size() << endl;
        cout << "Normal Queue Size: " << normalQueue.size() << endl;
        cout << "Total Players Registered: " << playerCount << endl;
        
        int checkedInCount = 0;
        int earlyBirdCount = 0;
        int wildcardCount = 0;
        
        for (int i = 0; i < playerCount; i++) {
            if (players[i].isCheckedIn) checkedInCount++;
            if (players[i].isEarlyBird) earlyBirdCount++;
            if (players[i].isWildcard) wildcardCount++;
        }
        
        cout << "Checked-in Players: " << checkedInCount << endl;
        cout << "Early Bird Registrations: " << earlyBirdCount << endl;
        cout << "Wildcard Entries: " << wildcardCount << endl;
        cout << "Players in Queues: " << (priorityQueue.size() + normalQueue.size()) << endl;
    }

public:
    RegistrationManager() {
        playerCount = 0;
        FileManager::loadPlayers(players, playerCount);
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
                    registerNewPlayer();
                    break;
                case 2:
                    checkInPlayer();
                    break;
                case 3:
                    viewRegistrationQueue();
                    break;
                case 4:
                    processNextPlayer();
                    break;
                case 5:
                    viewAllPlayers();
                    break;
                case 6:
                    addWildcardEntry();
                    break;
                case 7:
                    handleWithdrawal();
                    break;
                case 8:
                    displayQueueStats();
                    break;
                case 9:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid option! Please try again." << endl;
                    break;
            }
            
            if (choice != 9) {
                Utils::pauseScreen();
            }
            
        } while (choice != 9);
    }
};