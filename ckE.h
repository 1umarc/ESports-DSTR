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
    string teamNames[8];
    int teamCounts[8];
    int totalTeams = 0;
    int playerCount;
    Queue normalQueue;
    PriorityQueue priorityQueue;
    
    void displayMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    TOURNAMENT REGISTRATION & PLAYER QUEUEING" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Register New Player" << endl;
        cout << "2. Check-In Player" << endl;
        cout << "3. Last-Minute Check-In" << endl;
        cout << "4. Check Team Status" << endl;
        cout << "5. View All Registered Players" << endl;
        cout << "6. Handle Player Withdrawal" << endl;
        cout << "7. Replace Withdrawn Player (ADMIN)" << endl;
        cout << "8. Register Wildcard Player (ADMIN)" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter your choice: ";
    }


    void registerWildcardPlayer() {
        if (playerCount >= 100) {
            cout << "Maximum player capacity reached!" << endl;
            return;
        }

        Player newPlayer;
        cout << "\n=== WILDCARD PLAYER REGISTRATION ===" << endl;

        newPlayer.playerID = Utils::generatePlayerID(players, playerCount);
        cout << "Player ID: " << newPlayer.playerID << endl;

        cin.ignore();
        cout << "Enter player name: ";
        getline(cin, newPlayer.name);
        while (newPlayer.name.empty()) {
            cout << "Name cannot be empty. Please enter player name: ";
            getline(cin, newPlayer.name);
        }

        newPlayer.email = Utils::getEmailInput();

        cout << "Enter university: ";
        getline(cin, newPlayer.university);
        while (newPlayer.university.empty()) {
            cout << "University cannot be empty. Please enter university: ";
            getline(cin, newPlayer.university);
        }

        newPlayer.ranking = Utils::getRankingInput();
        newPlayer.registrationDate = Utils::getCurrentDate();

        // Explicitly set as Wildcard
        newPlayer.registrationType = "Wildcard";
        newPlayer.team = "BENCH";
        newPlayer.status = "Registered";
        newPlayer.wins = 0;
        newPlayer.losses = 0;

        // Add to array
        players[playerCount++] = newPlayer;

        loadTeamsFromPlayers();

        // Enqueue to priority queue with priority 2 (Wildcard)
        priorityQueue.enqueue(newPlayer.playerID + " - " + newPlayer.name + " (WILDCARD)", 2);

        cout << "Wildcard player registered successfully on " << newPlayer.registrationDate << endl;
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
    
    // void viewRegistrationQueue() {
    //     cout << "\n=== REGISTRATION QUEUE STATUS ===\n";

    //     // PRIORITY QUEUE
    //     cout << "\nPRIORITY QUEUE (Early-Bird & Wildcards):\n";

    //     if (priorityQueue.isEmpty()) {
    //         cout << "No players in priority queue.\n";
    //     } else {
    //         cout << left << setw(25) << "Player ID" 
    //             << setw(25) << "Name" 
    //             << setw(15) << "Priority Type" << endl;
    //         cout << string(65, '-') << endl;

    //         // Reconstruct display manually using players[]
    //         for (int i = 0; i < playerCount; i++) {
    //             if (players[i].status == "Registered" &&
    //                 (players[i].registrationType == "Early-Bird" || players[i].registrationType == "Wildcard")) {

    //                 cout << left << setw(25) << players[i].playerID 
    //                     << setw(25) << players[i].name 
    //                     << setw(15) << players[i].registrationType << endl;
    //             }
    //         }
    //     }

    //     // === NORMAL QUEUE ===
    //     cout << "\nNORMAL QUEUE (Regular Registration):\n";

    //     if (normalQueue.isEmpty()) {
    //         cout << "No players in normal queue.\n";
    //     } else {
    //         cout << left << setw(10) << "#" << "Player (ID Ending Suffix)" << endl;
    //         cout << string(40, '-') << endl;

    //         Queue tempQueue = normalQueue; // Copy to avoid modifying original
    //         int count = 1;
    //         while (!tempQueue.isEmpty()) {
    //             string suffix = tempQueue.deleteQueue();
    //             cout << left << setw(10) << count << "Player with ID ending in '" << suffix << "'\n";
    //             count++;
    //         }
    //     }
    // }
    
    void viewAllPlayers() {
        cout << "\n=== Early-Bird and Wildcard Players (REGISTRATION QUEUE)===" << endl;
        priorityQueue.display();

        cout << "\n=== Regular Players (REGISTRATION QUEUE) ===" << endl;
        normalQueue.display();

        cout << "\n=== PLAYERS LIST (By Queue Priority Order) ===" << endl;

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

        // --- EARLY-BIRD first ---
        cout << "\n-- Priority: Early-Bird --\n";
        for (int i = 0; i < playerCount; i++) {
            if (players[i].registrationType == "Early-Bird") {
                cout << left << setw(8) << players[i].playerID
                    << setw(20) << players[i].name
                    << setw(25) << players[i].university
                    << setw(8) << players[i].ranking
                    << setw(15) << players[i].registrationType
                    << setw(12) << players[i].status << endl;
            }
        }

        // --- Then: WILDCARD ---
        cout << "\n-- Priority: Wildcard --\n";
        for (int i = 0; i < playerCount; i++) {
            if (players[i].registrationType == "Wildcard") {
                cout << left << setw(8) << players[i].playerID
                    << setw(20) << players[i].name
                    << setw(25) << players[i].university
                    << setw(8) << players[i].ranking
                    << setw(15) << players[i].registrationType
                    << setw(12) << players[i].status << endl;
            }
        }

        // --- Lastly: REGULAR ---
        cout << "\n-- Regular Registration --\n";
        for (int i = 0; i < playerCount; i++) {
            if (players[i].registrationType == "Regular") {
                cout << left << setw(8) << players[i].playerID
                    << setw(20) << players[i].name
                    << setw(25) << players[i].university
                    << setw(8) << players[i].ranking
                    << setw(15) << players[i].registrationType
                    << setw(12) << players[i].status << endl;
            }
        }
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

        if (players[index].status == "WITHDRAWN") {
            cout << "Player is already marked as withdrawn.\n";
            return;
        }
        
        cout << "Withdrawing player: " << players[index].name << endl;
        players[index].status = "WITHDRAWN";
        players[index].team = "WITHDRAWN";

        cout << "Player marked as withdrawn successfully!" << endl;
        cout << "Withdrawal processed at: " << Utils::getCurrentTimestamp() << endl;
        
        FileManager::savePlayers(players, playerCount);
    }

    void processPriorityQueue() {
        cout << "\n=== PROCESSING PRIORITY QUEUE (Early-Bird / Wildcard) ===\n";

        // Display the current queue contents
        priorityQueue.display();

        // If empty, inform and exit
        if (priorityQueue.isEmpty()) {
            cout << "No pending registrations in priority queue.\n";
            return;
        }

        // Begin processing
        cout << "\n--- Processing and Checking-In ---\n";
        cout << left << setw(5)  << "#"
            << setw(35) << "Player Info"
            << setw(15) << "Status" << endl;
        cout << string(60, '-') << endl;

        int count = 1;
        while (!priorityQueue.isEmpty()) {
            string registration = priorityQueue.dequeue();

            // Update the corresponding player status to Checked-In
            for (int i = 0; i < playerCount; i++) {
                string expected = players[i].playerID + " - " + players[i].name;
                if (registration.find(expected) != string::npos) {
                    players[i].status = "Checked-In";
                    break;
                }
            }

            // Display processed player info
            cout << left << setw(5)  << count
                << setw(35) << registration
                << setw(15) << "Checked-In" << endl;
            count++;
        }

        cout << "\nAll priority registrations have been Checked-In!\n";
        FileManager::savePlayers(players, playerCount);
    }



    void processNormalQueue() {
        cout << "\n=== PROCESSING NORMAL QUEUE (Regular Players) ===\n";

        // Display the current queue contents
        normalQueue.display();

        if (normalQueue.isEmpty()) {
            cout << "No players in normal queue.\n";
            return;
        }

        cout << "\n--- Checking In Regular Players ---\n";
        cout << left << setw(5) << "#"
            << setw(40) << "Player Info"
            << setw(12) << "Status" << endl;
        cout << string(60, '-') << endl;

        int count = 1;
        while (!normalQueue.isEmpty()) {
            string registration = normalQueue.deleteQueue();
            bool matched = false;

            // Match player using full "ID - Name"
            for (int i = 0; i < playerCount; i++) {
                string expected = players[i].playerID + " - " + players[i].name;
                if (registration.find(expected) != string::npos) {
                    players[i].status = "Checked-In";
                    matched = true;
                    break;
                }
            }

            cout << left << setw(5) << count
                << setw(40) << registration
                << setw(12) << (matched ? "Checked-In" : "NOT FOUND") << endl;

            count++;
        }
        cout << "\nAll regular registrations have been processed.\n";
        FileManager::savePlayers(players, playerCount);
    }


    void replacePlayer() {
        cout << "\n--- Wildcard Player Team Assignment ---\n";
        
        // First, check if there are any wildcard players on the BENCH
        int wildcardCount = 0;
        cout << "Available Wildcard Players on BENCH:\n";
        for (int i = 0; i < playerCount; i++) {
            if (players[i].registrationType == "Wildcard" && players[i].team == "BENCH") {
                cout << (wildcardCount + 1) << ". " << players[i].playerID << " - " << players[i].name << endl;
                wildcardCount++;
            }
        }
        
        if (wildcardCount == 0) {
            cout << "No wildcard players available on the BENCH for team assignment.\n";
            return;
        }
        
        // Let user select which wildcard player to assign
        cout << "Select wildcard player to assign to team (1-" << wildcardCount << "): ";
        int wildcardChoice;
        cin >> wildcardChoice;
        cin.ignore();
        
        if (cin.fail() || wildcardChoice < 1 || wildcardChoice > wildcardCount) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid selection.\n";
            return;
        }
        
        // Find the selected wildcard player
        int selectedWildcardIndex = -1;
        int currentCount = 0;
        for (int i = 0; i < playerCount; i++) {
            if (players[i].registrationType == "Wildcard" && players[i].team == "BENCH") {
                currentCount++;
                if (currentCount == wildcardChoice) {
                    selectedWildcardIndex = i;
                    break;
                }
            }
        }
        
        if (selectedWildcardIndex == -1) {
            cout << "Error finding selected wildcard player.\n";
            return;
        }
        
        // Load teams from players
        loadTeamsFromPlayers();
        
        if (totalTeams == 0) {
            cout << "No teams available. Please create teams first.\n";
            return;
        }
        
        // Display available teams
        cout << "\nAvailable Teams:\n";
        for (int i = 0; i < totalTeams; i++) {
            cout << (i + 1) << ". " << teamNames[i] << " (" << teamCounts[i] << "/5)";
            if (teamCounts[i] >= 5) {
                cout << " - Full";
            } else {
                cout << " - Available";
            }
            cout << endl;
        }
        
        cout << "Select team for wildcard player (1-" << totalTeams << "): ";
        int teamChoice;
        cin >> teamChoice;
        cin.ignore();
        
        if (cin.fail() || teamChoice < 1 || teamChoice > totalTeams) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid team selection.\n";
            return;
        }
        
        string selectedTeam = teamNames[teamChoice - 1];
        int selectedTeamIndex = teamChoice - 1;
        
        // Check if team is full
        if (teamCounts[selectedTeamIndex] >= 5) {
            cout << "\nTeam '" << selectedTeam << "' is full. Select a player to withdraw:\n";
            
            // Display players in the selected team
            int teamPlayerCount = 0;
            for (int i = 0; i < playerCount; i++) {
                if (players[i].team == selectedTeam) {
                    cout << (teamPlayerCount + 1) << ". " << players[i].playerID << " - " << players[i].name 
                        << " (" << players[i].registrationType << ")" << endl;
                    teamPlayerCount++;
                }
            }
            
            cout << "Select player to withdraw (1-" << teamPlayerCount << "): ";
            int withdrawChoice;
            cin >> withdrawChoice;
            cin.ignore();
            
            if (cin.fail() || withdrawChoice < 1 || withdrawChoice > teamPlayerCount) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid player selection.\n";
                return;
            }
            
            // Find the player to withdraw
            int withdrawPlayerIndex = -1;
            int currentTeamCount = 0;
            for (int i = 0; i < playerCount; i++) {
                if (players[i].team == selectedTeam) {
                    currentTeamCount++;
                    if (currentTeamCount == withdrawChoice) {
                        withdrawPlayerIndex = i;
                        break;
                    }
                }
            }
            
            if (withdrawPlayerIndex == -1) {
                cout << "Error finding player to withdraw.\n";
                return;
            }
            
            // Withdraw the selected player (move to BENCH or remove)
            string withdrawnPlayerName = players[withdrawPlayerIndex].name;
            string withdrawnPlayerID = players[withdrawPlayerIndex].playerID;
            
            // Set withdrawn player's team to "BENCH" and status to "Withdrawn"
            players[withdrawPlayerIndex].team = "WITHDRAWN";
            players[withdrawPlayerIndex].status = "WITHDRAWN";
            
            // Assign wildcard player to the team
            players[selectedWildcardIndex].team = selectedTeam;
            
            cout << "Player replacement completed successfully!\n";
            cout << "Withdrawn: " << withdrawnPlayerName << " (" << withdrawnPlayerID << ") - moved to BENCH\n";
            cout << "Assigned: " << players[selectedWildcardIndex].name << " (" << players[selectedWildcardIndex].playerID << ") - assigned to team '" << selectedTeam << "'\n";
        }
        else {
            // Team has available slots, directly assign wildcard player
            players[selectedWildcardIndex].team = selectedTeam;
            
            cout << "Wildcard player assignment completed successfully!\n";
            cout << "Assigned: " << players[selectedWildcardIndex].name << " (" << players[selectedWildcardIndex].playerID << ") - assigned to team '" << selectedTeam << "'\n";
        }
        
        // Save updated player data
        FileManager::savePlayers(players, playerCount);
    }

    void loadTeamsFromPlayers() {
        totalTeams = 0;
        for (int i = 0; i < playerCount; i++) {
            string team = players[i].team;

            if (team.empty() || team == "BENCH" || team == "WITHDRAWN") continue;

            bool found = false;
            for (int j = 0; j < totalTeams; j++) {
                if (teamNames[j] == team) {
                    teamCounts[j]++;
                    found = true;
                    break;
                }
            }

            if (!found && totalTeams < 8) {
                teamNames[totalTeams] = team;
                teamCounts[totalTeams] = 1;
                totalTeams++;
            }
        }
    }

    void rebuildQueuesFromPlayers() {
        for (int i = 0; i < playerCount; i++) {
            if (players[i].status == "Registered") {
                string playerInfo = players[i].playerID + " - " + players[i].name;

                if (players[i].registrationType == "Early-Bird") {
                    priorityQueue.enqueue(playerInfo, 1);
                }
                else if (players[i].registrationType == "Wildcard") {
                    priorityQueue.enqueue(playerInfo + " (WILDCARD)", 2);
                }
                else if (players[i].registrationType == "Regular") {
                    normalQueue.addQueue(playerInfo);  // 
                }
            }
        }
    }

    void lastMinuteCheckIn() {
        cout << "\n=== LAST-MINUTE CHECK-IN ===" << endl;
        
        // Display current time
        cout << "Current time: " << Utils::getCurrentTimestamp() << endl;
        cout << "Tournament start time: [Set your tournament start time]" << endl;
        
        string playerID;
        cout << "Enter Player ID for last-minute check-in: ";
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
        
        if (players[index].status == "WITHDRAWN") {
            cout << "Player " << players[index].name << " has withdrawn and cannot check-in!" << endl;
            return;
        }
        
        // Mark as late check-in
        players[index].status = "Late Check-In";
        
        cout << "LAST-MINUTE CHECK-IN SUCCESSFUL!" << endl;
        cout << "Player: " << players[index].name << endl;
        cout << "Team: " << players[index].team << endl;
        cout << "Registration Type: " << players[index].registrationType << endl;
        cout << "Check-in time: " << Utils::getCurrentTimestamp() << endl;
        cout << "*** PLAYER MARKED FOR PRIORITY PROCESSING ***" << endl;
        
        FileManager::savePlayers(players, playerCount);
    }


    // Comprehensive tournament readiness check
    void tournamentReadinessCheck() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    TOURNAMENT READINESS VERIFICATION" << endl;
        cout << string(60, '=') << endl;
        
        loadTeamsFromPlayers();
        
        bool tournamentReady = true;
        int totalPlayersNeeded = 0;
        int totalPlayersRegistered = 0;
        int totalPlayersCheckedIn = 0;
        int totalPlayersWithdrawn = 0;
        int completeTeams = 0;
        
        // Team Analysis
        cout << "\n--- TEAM STATUS ANALYSIS ---" << endl;
        cout << left << setw(20) << "Team Name" 
            << setw(10) << "Members" 
            << setw(15) << "Checked-In" 
            << setw(15) << "Status" << endl;
        cout << string(70, '-') << endl;
        
        for (int i = 0; i < totalTeams; i++) {
            int checkedInCount = 0;
            int withdrawnCount = 0;
            
            // Count checked-in players for this team
            for (int j = 0; j < playerCount; j++) {
                if (players[j].team == teamNames[i]) {
                    if (players[j].status == "Checked-In" || players[j].status == "Late Check-In") {
                        checkedInCount++;
                    } else if (players[j].status == "WITHDRAWN") {
                        withdrawnCount++;
                    }
                }
            }
            
            cout << left << setw(20) << teamNames[i]
                << setw(10) << (to_string(teamCounts[i]) + "/5")
                << setw(15) << (to_string(checkedInCount) + "/" + to_string(teamCounts[i]));
            
            if (teamCounts[i] == 5 && checkedInCount == 5) {
                cout << setw(15) << "READY" << endl;
                completeTeams++;
            } else if (teamCounts[i] < 5) {
                cout << setw(15) << "INCOMPLETE" << endl;
                tournamentReady = false;
            } else if (checkedInCount < teamCounts[i]) {
                cout << setw(15) << "PENDING CHECK-IN" << endl;
                tournamentReady = false;
            }
            
            totalPlayersNeeded += 5;
            totalPlayersRegistered += teamCounts[i];
            totalPlayersCheckedIn += checkedInCount;
            totalPlayersWithdrawn += withdrawnCount;
        }
        
        // Overall Statistics
        cout << "\n--- TOURNAMENT STATISTICS ---" << endl;
        cout << "Total Teams: " << totalTeams << "/8" << endl;
        cout << "Complete Teams: " << completeTeams << "/" << totalTeams << endl;
        cout << "Players Needed: " << totalPlayersNeeded << endl;
        cout << "Players Registered: " << totalPlayersRegistered << endl;
        cout << "Players Checked-In: " << totalPlayersCheckedIn << endl;
        cout << "Players Withdrawn: " << totalPlayersWithdrawn << endl;
        
        // Queue Status
        cout << "\n--- QUEUE STATUS ---" << endl;
        cout << "Priority Queue (Pending): " << priorityQueue.size() << endl;
        cout << "Normal Queue (Pending): " << normalQueue.size() << endl;
        
        // Final Verdict
        cout << "\n" << string(60, '=') << endl;
        if (tournamentReady && totalTeams >= 4) { // Minimum 4 teams for tournament
            cout << "TOURNAMENT IS READY TO START!" << endl;
            cout << "All teams are complete and all players are checked-in." << endl;
        } else {
            cout << "TOURNAMENT NOT READY!" << endl;
            cout << "Issues to resolve:" << endl;
            
            if (totalTeams < 4) {
                cout << "- Need at least 4 complete teams (currently have " << completeTeams << ")" << endl;
            }
            if (totalPlayersRegistered < totalPlayersNeeded) {
                cout << "- Missing " << (totalPlayersNeeded - totalPlayersRegistered) << " players" << endl;
            }
            if (totalPlayersCheckedIn < totalPlayersRegistered - totalPlayersWithdrawn) {
                cout << "- " << (totalPlayersRegistered - totalPlayersWithdrawn - totalPlayersCheckedIn) << " players need to check-in" << endl;
            }
            
            // Suggest actions
            cout << "\nSuggested Actions:" << endl;
            if (priorityQueue.size() > 0 || normalQueue.size() > 0) {
                cout << "-Process remaining registrations" << endl;
            }
            if (totalPlayersRegistered < totalPlayersNeeded) {
                cout << "-Use wildcard replacements" << endl;
            }
            cout << "-Ensure all players check-in before tournament start" << endl;
        }
        cout << string(60, '=') << endl;
    }

    void checkInFunction() {
        int choice;

        do{
            cout << "\n--- CHECK-IN PLAYER ---" << endl;
            cout << "1. Check-In Player" << endl;
            cout << "2. Process All Priority Registrations" << endl;
            cout << "3. Process All Regular Registrations" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch(choice){
                case 1:
                    checkInPlayer();
                    break;
                case 2:
                    processPriorityQueue();
                    break;
                case 3:
                    processNormalQueue();
                    break;
                case 0:
                    displayMenu();
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
            }
            if (choice != 0) {
                Utils::pauseScreen();
            }
        } while (choice != 0);
        
    }

public:
    RegistrationManager() {
        playerCount = 0;
        FileManager::loadPlayers(players, playerCount);
        loadTeamsFromPlayers();
        rebuildQueuesFromPlayers();  
    }

    void registerNewPlayer() {
        // Check if player limit has been reached
        if (playerCount >= 50) {
            cout << "Maximum player capacity reached!" << endl;
            return;
        }

        Player newPlayer;
        cout << "\n=== PLAYER REGISTRATION ===" << endl;

        // Generate unique player ID (e.g., P001, P002)
        newPlayer.playerID = Utils::generatePlayerID(players, playerCount);
        cout << "Player ID: " << newPlayer.playerID << endl;

        // Collect basic details
        cin.ignore();
        cout << "Enter player name: ";
        getline(cin, newPlayer.name);

        while (newPlayer.name.empty()) {
            cout << "Name cannot be empty. Please enter player name: ";
            getline(cin, newPlayer.name);
        }

        // Collect validated email
        newPlayer.email = Utils::getEmailInput();

        cout << "Enter university: ";
        getline(cin, newPlayer.university);

        while (newPlayer.university.empty()) {
            cout << "University cannot be empty. Please enter university: ";
            getline(cin, newPlayer.university);
        }

        // Collect validated ranking
        newPlayer.ranking = Utils::getRankingInput();

        // Capture registration date
        newPlayer.registrationDate = Utils::getCurrentDate();

        // Auto-assign registration type: Early-Bird if P001–P015
        int numericID = stoi(newPlayer.playerID.substr(1));
        if (numericID <= 15)
            newPlayer.registrationType = "Early-Bird";
        else
            newPlayer.registrationType = "Regular";

        // Initialize player status and record
        newPlayer.status = "Registered";
        newPlayer.wins = 0;
        newPlayer.losses = 0;

        // Add player to array
        players[playerCount++] = newPlayer;

        // Refresh current teams
        loadTeamsFromPlayers();

        // Place player in appropriate queue
        if (newPlayer.registrationType == "Early-Bird") {
            priorityQueue.enqueue(newPlayer.playerID + " - " + newPlayer.name, 1);
        } else {
            normalQueue.addQueue(newPlayer.playerID + " - " + newPlayer.name);
        }

        cout << "Player registered successfully as a " << newPlayer.registrationType 
            << " on " << newPlayer.registrationDate << endl;

        // Skip team assignment for Wildcard players
        if (newPlayer.registrationType != "Wildcard") {
            while (true) {
                loadTeamsFromPlayers();

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

                    bool exists = false;
                    for (int i = 0; i < totalTeams; i++) {
                        if (teamNames[i] == newTeamName) {
                            exists = true;
                            break;
                        }
                    }

                    if (exists) {
                        cout << "Team name already exists. Please choose a different name.\n";
                        continue;
                    }

                    // Assign new team to player
                    teamNames[totalTeams] = newTeamName;
                    teamCounts[totalTeams] = 1;
                    totalTeams++;
                    players[playerCount - 1].team = newTeamName;
                    cout << "Team '" << newTeamName << "' created and player assigned!\n";
                    break;
                }
                else if (option == 2) {
                    int availableTeams = 0;
                    cout << "\nAvailable teams:\n";

                    // Show all teams with availability
                    for (int i = 0; i < totalTeams; ++i) {
                        cout << (i + 1) << ". " << teamNames[i] << " (" << teamCounts[i] << "/5)";
                        if (teamCounts[i] < 5) {
                            cout << " - Available";
                            availableTeams++;
                        } else {
                            cout << " - Full";
                        }
                        cout << "\n";
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

                    if (choice >= 1 && choice <= totalTeams) {
                        if (teamCounts[choice - 1] < 5) {
                            string selected = teamNames[choice - 1];

                            players[playerCount - 1].team = selected;
                            teamCounts[choice - 1]++;

                            cout << "Player added to team '" << selected << "' successfully!\n";
                            break;
                        } else {
                            cout << "Team is full. Please choose a team with available slots.\n";
                        }
                    } else {
                        cout << "Invalid selection. Please choose a valid team number.\n";
                    }
                }
                else {
                    cout << "Invalid option. Please enter 1 or 2.\n";
                }
            }
        }

        // Save updated player list to CSV
        FileManager::savePlayers(players, playerCount);
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
                    checkInFunction();
                    break;
                case 3:
                    lastMinuteCheckIn();
                    break;
                case 4:
                    tournamentReadinessCheck();
                    break;
                case 5:
                    viewAllPlayers();
                    break;
                case 6:
                    handleWithdrawal();
                    break;
                case 7:
                    replacePlayer();
                    break;
                case 8:
                    registerWildcardPlayer();
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