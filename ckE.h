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
    TeamManager teamManager;
    
    void displayMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    TOURNAMENT REGISTRATION & PLAYER QUEUEING" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Register New Player" << endl;
        cout << "2. Check-In Player" << endl;
        cout << "3. View Registration Queue" << endl;
        cout << "4. Process Next Player in Queue" << endl;
        cout << "5. View All Registered Players" << endl;
        cout << "6. Process All Priority Registrations" << endl;
        cout << "7. Handle Player Withdrawal" << endl;
        cout << "8. Display Queue Statistics" << endl;
        cout << "9. Replace Withdrawn Player\n";
        cout << "0. Back to Main Menu" << endl;
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

        newPlayer.playerID = Utils::generatePlayerID(players, playerCount);
        cout << "Player ID: " << newPlayer.playerID << endl;

        cin.ignore();
        cout << "Enter player name: ";
        getline(cin, newPlayer.name);

        // Validate name is not empty
        while (newPlayer.name.empty()) {
            cout << "Name cannot be empty. Please enter player name: ";
            getline(cin, newPlayer.name);
        }

        // Get validated email (automatically converted to lowercase)
        newPlayer.email = Utils::getEmailInput();

        cout << "Enter university: ";
        getline(cin, newPlayer.university);
        
        // Validate university is not empty
        while (newPlayer.university.empty()) {
            cout << "University cannot be empty. Please enter university: ";
            getline(cin, newPlayer.university);
        }

        // Get validated ranking
        newPlayer.ranking = Utils::getRankingInput();

        newPlayer.registrationDate = Utils::getCurrentDate();

        int typeChoice;
        do {
            cout << "Registration Type:\n1. Early-Bird\n2. Wildcard\n3. Regular\nChoose: ";
            cin >> typeChoice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter 1, 2, or 3.\n";
                continue;
            }
            
            if (typeChoice < 1 || typeChoice > 3) {
                cout << "Please select 1, 2, or 3.\n";
            }
        } while (typeChoice < 1 || typeChoice > 3);

        if (typeChoice == 1) newPlayer.registrationType = "Early-Bird";
        else if (typeChoice == 2) newPlayer.registrationType = "Wildcard";
        else newPlayer.registrationType = "Regular";

        newPlayer.status = "Registered";
        newPlayer.wins = 0;
        newPlayer.losses = 0;

        // Add player to array first
        players[playerCount++] = newPlayer;

        // Queue based on priority
        if (newPlayer.registrationType == "Early-Bird")
            priorityQueue.enqueue(newPlayer.playerID + " - " + newPlayer.name, 1);
        else if (newPlayer.registrationType == "Wildcard")
            priorityQueue.enqueue(newPlayer.playerID + " - " + newPlayer.name + " (WILDCARD)", 2);
        else
            normalQueue.addQueue(newPlayer.playerID[3]);

        cout << "Player registered successfully on " << newPlayer.registrationDate << endl;

        // Team assignment loop
        while (true) {
            string teamNames[8];
            int teamCounts[8];
            int totalTeams = 0;

            teamManager.getTeamInfo(teamNames, teamCounts, totalTeams);

            cout << "\n--- Team Assignment ---\n";
            cout << "1. Create New Team\n2. Join Existing Team\nEnter option: ";
            int option;
            cin >> option;
            cin.ignore();

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter 1 or 2.\n";
                continue;
            }

            if (option == 1) {
                if (totalTeams >= 8) {
                    cout << "Maximum number of teams (8) already exists.\n";
                    continue;
                }

                string newTeamName;
                cout << "Enter new team name: ";
                getline(cin, newTeamName);
                
                if (newTeamName.empty()) {
                    cout << "Team name cannot be empty.\n";
                    continue;
                }

                if (teamManager.teamExists(newTeamName)) {
                    cout << "Team already exists. Try joining it instead.\n";
                    continue;
                }

                if (teamManager.createNewTeam(newTeamName)) {
                    players[playerCount - 1].team = newTeamName;
                    cout << "Team '" << newTeamName << "' created and player assigned!\n";
                    break;
                } else {
                    cout << "Failed to create new team.\n";
                }
            }
            else if (option == 2) {
                int availableTeams = 0;
                cout << "\nAvailable teams with slots:\n";
                for (int i = 0; i < totalTeams; ++i) {
                    if (teamCounts[i] < 5) {
                        cout << (i + 1) << ". " << teamNames[i] << " (" << teamCounts[i] << "/5)\n";
                        availableTeams++;
                    }
                }

                if (availableTeams == 0) {
                    cout << "No teams with available slots. You must create a new team.\n";
                    continue;
                }

                cout << "Enter team number to join: ";
                int choice;
                cin >> choice;
                cin.ignore();
                
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Please enter a number.\n";
                    continue;
                }

                if (choice >= 1 && choice <= totalTeams && teamCounts[choice - 1] < 5) {
                    string selected = teamNames[choice - 1];
                    if (teamManager.addPlayerToTeam(selected)) {
                        players[playerCount - 1].team = selected;
                        cout << "Player added to team '" << selected << "' successfully!\n";
                        break;
                    } else {
                        cout << "Team is full. Try another.\n";
                    }
                } else {
                    cout << "Invalid selection.\n";
                }
            }
            else {
                cout << "Invalid option. Please enter 1 or 2.\n";
            }
        }

        // Save players to CSV AFTER team assignment is complete
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
        
        if (players[index].status == "Checked-In") {
            cout << "Player " << players[index].name << " is already checked in!" << endl;
            return;
        }

        players[index].status = "Checked-In";


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
            << setw(15) << "Reg Type" 
            << setw(12) << "Status" << endl;

        cout << string(90, '-') << endl;

        for (int i = 0; i < playerCount; i++) {
            cout << left << setw(8) << players[i].playerID
                << setw(20) << players[i].name
                << setw(25) << players[i].university
                << setw(8) << players[i].ranking
                << setw(15) << players[i].registrationType
                << setw(12) << players[i].status << endl;
        }
    }
    
    void addWildcardEntry() {
        if (playerCount >= 100) {
            cout << "Maximum player capacity reached!" << endl;
            return;
        }

        Player wildcard;
        cout << "\n=== WILDCARD ENTRY ===" << endl;

        wildcard.playerID = Utils::generatePlayerID(players, playerCount);
        cout << "Wildcard Player ID: " << wildcard.playerID << endl;

        cin.ignore();
        cout << "Enter player name: ";
        getline(cin, wildcard.name);

        cout << "Enter email: ";
        getline(cin, wildcard.email);

        cout << "Enter university: ";
        getline(cin, wildcard.university);

        cout << "Enter ranking (1000-2000): ";
        cin >> wildcard.ranking;

        wildcard.registrationDate = Utils::getCurrentDate();
        wildcard.registrationType = "Wildcard";
        wildcard.status = "Registered";
        wildcard.wins = 0;
        wildcard.losses = 0;

        players[playerCount++] = wildcard;

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
            if (players[i].status == "Checked-In") checkedInCount++;
            if (players[i].registrationType == "Early-Bird") earlyBirdCount++;
            if (players[i].registrationType == "Wildcard") wildcardCount++;
        }
        
        cout << "Checked-in Players: " << checkedInCount << endl;
        cout << "Early Bird Registrations: " << earlyBirdCount << endl;
        cout << "Wildcard Entries: " << wildcardCount << endl;
        cout << "Players in Queues: " << (priorityQueue.size() + normalQueue.size()) << endl;
    }

    void processPriorityRegistrations() {
        cout << "\n--- Processing Priority Registrations ---\n";
        
        if (priorityQueue.isEmpty()) {
            cout << "No pending registrations in priority queue.\n";
            return;
        }
        
        cout << "Processing registrations in priority order:\n";
        int count = 1;
        
        while (!priorityQueue.isEmpty()) {
            string registration = priorityQueue.dequeue();
            cout << count << ". " << registration << " - PROCESSED\n";
            count++;
        }
        
        cout << "All priority registrations have been processed!\n";
    }

    void replacePlayer() {
        string withdrawnID, replacementID, replacementName, replacementEmail;
        int regChoice;
        string regType;

        cout << "\n--- Player Replacement ---\n";
        cout << "Enter Withdrawn Player ID to replace: ";
        cin >> withdrawnID;

        int index = Utils::findPlayerIndex(players, playerCount, withdrawnID);
        if (index == -1 || players[index].status != "Withdrawn") {
            cout << "Error: Withdrawn player not found or player is not withdrawn!\n";
            return;
        }

        // Validate replacement ID format and uniqueness
        while (true) {
            cout << "Enter Replacement Player ID (Format: PXXX e.g., P010): ";
            cin >> replacementID;

            if (replacementID.length() == 4 && replacementID[0] == 'P' &&
                isdigit(replacementID[1]) && isdigit(replacementID[2]) && isdigit(replacementID[3])) {
                
                if (Utils::findPlayerIndex(players, playerCount, replacementID) != -1) {
                    cout << "Error: Replacement player ID already exists!\n";
                    continue;
                }
                break;
            } else {
                cout << "Invalid ID format! Please enter again.\n";
            }
        }

        cin.ignore(); // Clear buffer
        cout << "Enter Replacement Player Name: ";
        getline(cin, replacementName);

        while (true) {
            cout << "Enter Replacement Email (must end with @gmail.com): ";
            getline(cin, replacementEmail);

            if (replacementEmail.size() >= 10 &&
                replacementEmail.substr(replacementEmail.size() - 10) == "@gmail.com") {
                break;
            } else {
                cout << "Invalid email format! Please enter a Gmail address.\n";
            }
        }

        while (true) {
            cout << "\nSelect Registration Type:\n1. Early-Bird\n2. Wildcard\n3. Regular\nChoose: ";
            cin >> regChoice;

            if (regChoice == 1) {
                regType = "Early-Bird";
                break;
            } else if (regChoice == 2) {
                regType = "Wildcard";
                break;
            } else if (regChoice == 3) {
                regType = "Regular";
                break;
            } else {
                cout << "Invalid choice! Try again.\n";
            }
        }

        if (playerCount >= 100) {
            cout << "Maximum player capacity reached!\n";
            return;
        }

        Player replacement;
        replacement.playerID = replacementID;
        replacement.name = replacementName;
        replacement.email = replacementEmail;
        replacement.university = players[index].university; 
        replacement.team = players[index].team;             
        replacement.ranking = Utils::getRankingInput();
        replacement.registrationDate = Utils::getCurrentDate();
        replacement.registrationType = regType;
        replacement.status = "Registered";
        replacement.wins = 0;
        replacement.losses = 0;

        players[playerCount++] = replacement;

        // Queue accordingly
        int priority = (regType == "Early-Bird") ? 1 : (regType == "Wildcard") ? 2 : 3;

        if (priority == 1 || priority == 2)
            priorityQueue.enqueue(replacementID + " - " + replacementName + " (Replacement)", priority);
        else
            normalQueue.addQueue(replacementID[3]); // using last digit like before

        FileManager::savePlayers(players, playerCount);

        cout << "Replacement registered for " << players[index].name << " successfully!\n";
        cout << "New player: " << replacement.name << " | Team: " << replacement.team << "\n";
    }


public:
    RegistrationManager() {
        playerCount = 0;
        FileManager::loadPlayers(players, playerCount);

        // Rebuild team data from loaded players
        for (int i = 0; i < playerCount; i++) {
            string team = players[i].team;
            if (!team.empty()) {
                if (!teamManager.teamExists(team)) {
                    teamManager.createNewTeam(team);
                } else {
                    teamManager.addPlayerToTeam(team);
                }
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
                    processPriorityRegistrations();
                    break;
                case 7:
                    handleWithdrawal();
                    break;
                case 8:
                    displayQueueStats();
                    break;
                case 9:
                    replacePlayer();
                    break;
                case 0:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid option! Please try again." << endl;
                    break;
            }
            
            if (choice != 0) {
                Utils::pauseScreen();
            }
            
        } while (choice != 0);
    }
};