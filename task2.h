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
    Player players[50];
    string teamNames[8];
    int teamCounts[5];
    int totalTeams = 0;
    int playerCount;
    Queue normalQueue;  // FIFO queue for regular players
    PriorityQueue priorityQueue;  // Priority queue for early-bird (priority 1) and wildcard players (priority 2)
    
    // Shows menu options
    void displayPlayerMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    TOURNAMENT REGISTRATION & PLAYER QUEUEING" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Check-In Player" << endl;
        cout << "2. View All Registered Players" << endl;
        cout << "3. Player Withdrawal" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter your choice: ";
    }

    // Shows menu options for administrators
    void displayAdminMenu() 
    {
        cout << "\n" << string(60, '=') << endl;
        cout << "    TOURNAMENT REGISTRATION & PLAYER QUEUEING" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Check-In Player" << endl;
        cout << "2. Replace Withdrawn Player" << endl;
        cout << "3. Register Wildcard Player" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter your choice: ";
    }

    // Allows admins to register special wildcard players
    void registerWildcardPlayer() {
        if (playerCount >= 50) {
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

        // Sets team as "BENCH" initially
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
    
    // Allows players to check themselves
    void checkInPlayer(const string& loggedInPlayerID) {
        string playerID;
        cout << "\n=== PLAYER CHECK-IN ===" << endl;
        cout << "Enter Player ID: ";
        cin >> playerID;

        // Ensure the logged-in player is checking in themselves
        if (playerID != loggedInPlayerID) {
            cout << "Error: You are only allowed to check in using your own Player ID!" << endl;
            return;
        }

        int index = Utils::findPlayerIndex(players, playerCount, playerID);
        if (index == -1) {
            cout << "Player not found!" << endl;
            return;
        }

        // Prevents duplicate check-ins
        if (players[index].status == "Checked-In") {
            cout << "Player " << players[index].name << " is already checked in!" << endl;
            return;
        }

        players[index].status = "Checked-In";
        cout << "Player " << players[index].name << " checked in successfully!" << endl;
        cout << "Check-in time: " << Utils::getCurrentTimestamp() << endl;

        // Saves updated data to file
        FileManager::savePlayers(players, playerCount);
    }
    
    // Displays all players organized by registration priority
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
    
    // Processes player withdrawals
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

        // Checks if already withdrawn
        if (players[index].status == "WITHDRAWN") {
            cout << "Player is already marked as withdrawn.\n";
            return;
        }
        
        // Updates team assignment and status to "WITHDRAWN"
        cout << "Withdrawing player: " << players[index].name << endl;
        players[index].status = "WITHDRAWN";
        players[index].team = "WITHDRAWN";

        cout << "Player marked as withdrawn successfully!" << endl;
        cout << "Withdrawal processed at: " << Utils::getCurrentTimestamp() << endl;
        
        FileManager::savePlayers(players, playerCount);
    }

    // Processes all early-bird and wildcard players for check-in
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

    // Processes all regular players for check-in
    void processNormalQueue() {
        cout << "\n=== PROCESSING NORMAL QUEUE (Regular Players) ===\n";

        // Show the queue contents before processing
        normalQueue.display();

        if (normalQueue.isEmpty()) {
            cout << "No players in normal queue.\n";
            return;
        }

        // Display table headers for processing output
        cout << "\n--- Checking In Regular Players ---\n";
        cout << left << setw(5) << "#"
            << setw(40) << "Player Info"
            << setw(12) << "Status" << endl;
        cout << string(60, '-') << endl;

        int count = 1;

        // Process each player in the normal queue
        while (!normalQueue.isEmpty()) {
            string registration = normalQueue.dequeue(); // Get and remove first in queue
            bool matched = false;

            // Try to match queue entry with registered players
            for (int i = 0; i < playerCount; i++) {
                string expected = players[i].playerID + " - " + players[i].name;
                if (registration.find(expected) != string::npos) {
                    players[i].status = "Checked-In"; // Mark as checked-in
                    matched = true;
                    break;
                }
            }

            // Display check-in result
            cout << left << setw(5) << count
                << setw(40) << registration
                << setw(12) << (matched ? "Checked-In" : "NOT FOUND") << endl;

            count++;
        }

        cout << "\nAll regular registrations have been processed.\n";

        // Save updated player statuses to CSV
        FileManager::savePlayers(players, playerCount);
    }

    // Allows admins to assign wildcard players to teams
    void replacePlayer() {
        cout << "\n--- Wildcard Player Team Assignment ---\n";

        // Show available wildcard players on BENCH
        int wildcardCount = 0;
        cout << "Available Wildcard Players on BENCH:\n";
        // Loop through all players to find wildcards on BENCH
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

        // Prompt admin to select a wildcard player
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

        loadTeamsFromPlayers();

        // Check if any teams exist
        if (totalTeams == 0) {
            cout << "No teams available. Please create teams first.\n";
            return;
        }

        cout << "\nAvailable Teams:\n";
        for (int i = 0; i < totalTeams; i++) {
            cout << (i + 1) << ". " << teamNames[i] << " (" << teamCounts[i] << "/5)";
            cout << (teamCounts[i] >= 5 ? " - Full" : " - Available") << endl;
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

        if (teamCounts[selectedTeamIndex] >= 5) {
            cout << "\nTeam '" << selectedTeam << "' is full. Select a player to withdraw:\n";

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

            string withdrawnPlayerName = players[withdrawPlayerIndex].name;
            string withdrawnPlayerID = players[withdrawPlayerIndex].playerID;

            // Update withdrawn player’s status and team
            players[withdrawPlayerIndex].team = "WITHDRAWN";
            players[withdrawPlayerIndex].status = "WITHDRAWN";

            // Assign wildcard player to the chosen team
            players[selectedWildcardIndex].team = selectedTeam;

            cout << "Player replacement completed successfully!\n";
            cout << "Withdrawn: " << withdrawnPlayerName << " (" << withdrawnPlayerID << ") - set to WITHDRAWN\n";
            cout << "Assigned: " << players[selectedWildcardIndex].name << " (" << players[selectedWildcardIndex].playerID << ") to team '" << selectedTeam << "'\n";
        }
        else {
            // Team has space; assign directly
            players[selectedWildcardIndex].team = selectedTeam;
            cout << "Wildcard player assigned to team '" << selectedTeam << "' successfully!\n";
        }

        // Save updated data
        FileManager::savePlayers(players, playerCount);
    }

    // Rebuilds team information from player data
    void loadTeamsFromPlayers() 
    {
        // Scans all players to extract unique team names
        totalTeams = 0;
        for (int i = 0; i < playerCount; i++) 
        {
            string team = players[i].team;

            // Ignores special statuses: "BENCH", "WITHDRAWN", empty teams
            if (team.empty() || team == "BENCH" || team == "WITHDRAWN") continue;

            bool found = false;
            for (int j = 0; j < totalTeams; j++) 
            {
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

    // Reconstructs queues from saved player data (used during startup
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
                    normalQueue.enqueue(playerInfo);  // 
                }
            }
        }
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
        
        // Shows each team's status (members, checked-in count, readiness)
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
                    if (players[j].status == "Checked-In") {
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
        
        // Total teams, complete teams, players needed/registered/checked-in/withdrawn
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
        
        // Final Verdict all players checked-in to be ready
        cout << "\n" << string(60, '=') << endl;
        if (tournamentReady && totalTeams >= 8) { 
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

    // Admin interface for processing registrations
    void checkInFunction() {
        int choice;

        do{
            cout << "\n--- CHECK-IN PLAYER ---" << endl;
            cout << "1. Process All Priority Registrations" << endl;
            cout << "2. Process All Regular Registrations" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch(choice){
                case 1:
                    processPriorityQueue();
                    break;
                case 2:
                    processNormalQueue();
                    break;
                case 0:
                    displayPlayerMenu();
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
    // Initializes the system when program starts (Constructor)
    RegistrationManager() {
        playerCount = 0;
        FileManager::loadPlayers(players, playerCount);
        loadTeamsFromPlayers();
        rebuildQueuesFromPlayers();  
    }

    // Main player registration function to sign in new player
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
            normalQueue.enqueue(newPlayer.playerID + " - " + newPlayer.name);
        }

        cout << "Player registered successfully as a " << newPlayer.registrationType 
            << " on " << newPlayer.registrationDate << endl;

        // Skip team assignment for Wildcard players
        if (newPlayer.registrationType != "Wildcard") {
            while (true) {
                loadTeamsFromPlayers();

                cout << "\n--- Team Assignment ---\n";
                cout << "1. Create New Team\n2. Join Existing Team\n0. Exit\nEnter option: ";
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
                else if (option == 0) {
                    cout << "Returning to main menu...\n";
                    break;
                }
                else {
                    cout << "Invalid option. Please enter 1 or 2.\n";
                }
            }
        }

        // Save updated player list to CSV
        FileManager::savePlayers(players, playerCount);
    }
    
    void run(string id) {
        int choice;
        string currentPlayerID = id; 
        
        do {
            displayPlayerMenu();
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            
            switch (choice) {
                case 1:
                    checkInPlayer(currentPlayerID);
                    break;
                case 2:
                    viewAllPlayers();
                    break;
                case 3:
                    handleWithdrawal();
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

    void adminMenu()
    {
        replacePlayer();
        registerWildcardPlayer();

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
                    checkInFunction();
                    break;
                case 2:
                    replacePlayer();
                    break;
                case 3:
                    registerWildcardPlayer();
                    break;
                case 4:
                    tournamentReadinessCheck();
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

    void playerMenu(string id)
    {
        int choice;
        string currentPlayerID = id; 
        
        do {
            displayPlayerMenu();
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            
            switch (choice) {
                case 1:
                    checkInPlayer(currentPlayerID);
                    break;
                case 2:
                    viewAllPlayers();
                    break;
                case 3:
                    handleWithdrawal();
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