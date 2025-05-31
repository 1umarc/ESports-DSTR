#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "structs.h"
#include "helper.h"
#include "Stack.h"
#include "Queue.h"

using namespace std;

class MatchScheduler {
private:
    Stack matchHistory;           // Stack to store last matches played (for quick access to recent matches)
    Queue matchQueue;            // Queue to manage match progression order
    Player players[100];         // Array to store players
    Match matches[200];          // Array to store matches
    int playerCount;
    int matchCount;
    
public:
    MatchScheduler() {
        playerCount = 0;
        matchCount = 0;
        // Load players from CSV
        FileManager::loadPlayers(players, playerCount);
    }
    
    ~MatchScheduler() {
        // Save any updated player data
        FileManager::savePlayers(players, playerCount);
    }
    
    void runTask() {
        int choice;
        
        cout << "\n" << string(60, '=') << endl;
        cout << "    TASK 1: MATCH SCHEDULING & PLAYER PROGRESSION" << endl;
        cout << string(60, '=') << endl;
        
        do {
            cout << "\nMatch Scheduling Menu:" << endl;
            cout << "1. View Available Players" << endl;
            cout << "2. Schedule New Match" << endl;
            cout << "3. Process Match Queue" << endl;
            cout << "4. View Match History (Recent Matches)" << endl;
            cout << "5. Simulate Match Result" << endl;
            cout << "6. View Tournament Bracket" << endl;
            cout << "7. Update Player Rankings" << endl;
            cout << "0. Return to Main Menu" << endl;
            cout << string(40, '-') << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            
            switch (choice) {
                case 1:
                    viewAvailablePlayers();
                    break;
                case 2:
                    scheduleNewMatch();
                    break;
                case 3:
                    processMatchQueue();
                    break;
                case 4:
                    viewMatchHistory();
                    break;
                case 5:
                    simulateMatchResult();
                    break;
                case 6:
                    viewTournamentBracket();
                    break;
                case 7:
                    updatePlayerRankings();
                    break;
                case 0:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    break;
            }
        } while (choice != 0);
    }
    
private:
    void viewAvailablePlayers() {
        cout << "\n" << string(80, '=') << endl;
        cout << "                          AVAILABLE PLAYERS" << endl;
        cout << string(80, '=') << endl;
        
        if (playerCount == 0) {
            cout << "No players found in the database!" << endl;
            return;
        }
        
        cout << "ID     | Name              | University          | Ranking | W-L Record" << endl;
        cout << string(80, '-') << endl;
        
        for (int i = 0; i < playerCount; i++) {
            cout << players[i].playerID << " | ";
            cout << players[i].name;
            // Padding for name
            for (int j = players[i].name.length(); j < 17; j++) cout << " ";
            cout << " | ";
            cout << players[i].university;
            // Padding for university
            for (int j = players[i].university.length(); j < 19; j++) cout << " ";
            cout << " | ";
            cout << players[i].ranking;
            // Padding for ranking
            for (int j = to_string(players[i].ranking).length(); j < 7; j++) cout << " ";
            cout << " | ";
            cout << players[i].wins << "-" << players[i].losses << endl;
        }
        cout << string(80, '=') << endl;
        cout << "Total Players: " << playerCount << endl;
    }
    
    void scheduleNewMatch() {
        cout << "\n" << string(60, '=') << endl;
        cout << "                SCHEDULE NEW MATCH" << endl;
        cout << string(60, '=') << endl;
        
        if (playerCount < 2) {
            cout << "Not enough players to schedule a match!" << endl;
            return;
        }
        
        string player1ID, player2ID, stage;
        
        cout << "Enter Player 1 ID: ";
        cin >> player1ID;
        cout << "Enter Player 2 ID: ";
        cin >> player2ID;
        
        // Validate players exist
        int p1Index = Utils::findPlayerIndex(players, playerCount, player1ID);
        int p2Index = Utils::findPlayerIndex(players, playerCount, player2ID);
        
        if (p1Index == -1 || p2Index == -1) {
            cout << "One or both players not found!" << endl;
            return;
        }
        
        if (player1ID == player2ID) {
            cout << "Cannot schedule match with the same player!" << endl;
            return;
        }
        
        cout << "Select match stage:" << endl;
        cout << "1. Qualifier" << endl;
        cout << "2. Group Stage" << endl;
        cout << "3. Knockout" << endl;
        cout << "Enter choice: ";
        int stageChoice;
        cin >> stageChoice;
        
        switch (stageChoice) {
            case 1: stage = "Qualifier"; break;
            case 2: stage = "Group"; break;
            case 3: stage = "Knockout"; break;
            default: stage = "Qualifier"; break;
        }
        
        // Create new match
        Match newMatch;
        newMatch.matchID = Utils::generateMatchID();
        newMatch.player1ID = player1ID;
        newMatch.player2ID = player2ID;
        newMatch.matchDate = Utils::getCurrentDate();
        newMatch.stage = stage;
        
        // Add match to array
        matches[matchCount] = newMatch;
        matchCount++;
        
        // Add match ID to queue for processing (using first char of match ID)
        char matchIDChar = newMatch.matchID[1]; // Get second character of match ID
        matchQueue.addQueue(matchIDChar);
        
        cout << "\nMatch scheduled successfully!" << endl;
        cout << "Match ID: " << newMatch.matchID << endl;
        cout << "Players: " << players[p1Index].name << " vs " << players[p2Index].name << endl;
        cout << "Stage: " << stage << endl;
        cout << "Date: " << newMatch.matchDate << endl;
        cout << "Match added to processing queue." << endl;
    }
    
    void processMatchQueue() {
        cout << "\n" << string(60, '=') << endl;
        cout << "               PROCESS MATCH QUEUE" << endl;
        cout << string(60, '=') << endl;
        
        if (matchQueue.isEmpty()) {
            cout << "No matches in queue to process!" << endl;
            return;
        }
        
        cout << "Processing matches in queue order..." << endl;
        cout << "\nCurrent queue contents:" << endl;
        matchQueue.display();
        
        cout << "\nDo you want to process the next match? (y/n): ";
        char confirm;
        cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            char processedMatch = matchQueue.deleteQueue();
            cout << "\nProcessed match with ID ending in: " << processedMatch << endl;
            cout << "Match is now ready for play!" << endl;
            
            // Add to match history stack
            matchHistory.push(processedMatch);
            cout << "Match added to history stack." << endl;
        }
    }
    
    void viewMatchHistory() {
        cout << "\n" << string(60, '=') << endl;
        cout << "            RECENT MATCH HISTORY" << endl;
        cout << string(60, '=') << endl;
        
        if (matchHistory.isEmpty()) {
            cout << "No matches in history!" << endl;
            return;
        }
        
        cout << "Most recent match ID suffix: " << matchHistory.peek() << endl;
        cout << "\nNote: Stack stores match ID suffixes for quick access to recent matches." << endl;
        cout << "Full match details would be stored in the database." << endl;
        
        cout << "\nStack contents:" << endl;
        matchHistory.display();
        
        cout << "\nWould you like to view detailed match history? (y/n): ";
        char choice;
        cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            viewDetailedMatchHistory();
        }
    }
    
    void viewDetailedMatchHistory() {
        cout << "\n" << string(80, '=') << endl;
        cout << "                        DETAILED MATCH HISTORY" << endl;
        cout << string(80, '=') << endl;
        
        if (matchCount == 0) {
            cout << "No matches found!" << endl;
            return;
        }
        
        cout << "Match ID | Player 1     | Player 2     | Stage      | Date       | Status" << endl;
        cout << string(80, '-') << endl;
        
        for (int i = matchCount - 1; i >= 0 && i >= matchCount - 10; i--) {
            cout << matches[i].matchID << "   | ";
            
            // Find player names
            int p1Index = Utils::findPlayerIndex(players, playerCount, matches[i].player1ID);
            int p2Index = Utils::findPlayerIndex(players, playerCount, matches[i].player2ID);
            
            string p1Name = (p1Index != -1) ? players[p1Index].name : "Unknown";
            string p2Name = (p2Index != -1) ? players[p2Index].name : "Unknown";
            
            cout << p1Name;
            for (int j = p1Name.length(); j < 12; j++) cout << " ";
            cout << " | ";
            cout << p2Name;
            for (int j = p2Name.length(); j < 12; j++) cout << " ";
            cout << " | ";
            cout << matches[i].stage;
            for (int j = matches[i].stage.length(); j < 10; j++) cout << " ";
            cout << " | ";
            cout << matches[i].matchDate << " | ";
            cout << (matches[i].winner.empty() ? "Scheduled" : "Completed") << endl;
        }
        cout << string(80, '=') << endl;
    }
    
    void simulateMatchResult() {
        cout << "\n" << string(60, '=') << endl;
        cout << "              SIMULATE MATCH RESULT" << endl;
        cout << string(60, '=') << endl;
        
        if (matchCount == 0) {
            cout << "No matches scheduled!" << endl;
            return;
        }
        
        // Find incomplete matches
        int incompleteMatches[200];
        int incompleteCount = 0;
        
        for (int i = 0; i < matchCount; i++) {
            if (matches[i].winner.empty()) {
                incompleteMatches[incompleteCount] = i;
                incompleteCount++;
            }
        }
        
        if (incompleteCount == 0) {
            cout << "No incomplete matches to simulate!" << endl;
            return;
        }
        
        cout << "Incomplete matches:" << endl;
        for (int i = 0; i < incompleteCount; i++) {
            int matchIndex = incompleteMatches[i];
            cout << (i + 1) << ". " << matches[matchIndex].matchID 
                 << " - " << matches[matchIndex].stage << endl;
        }
        
        cout << "Select match to simulate (1-" << incompleteCount << "): ";
        int choice;
        cin >> choice;
        
        if (choice < 1 || choice > incompleteCount) {
            cout << "Invalid choice!" << endl;
            return;
        }
        
        int selectedMatch = incompleteMatches[choice - 1];
        Match& match = matches[selectedMatch];
        
        // Simulate match result
        int score1 = rand() % 3 + 1; // Random score 1-3
        int score2 = rand() % 3 + 1;
        
        match.player1Score = score1;
        match.player2Score = score2;
        
        if (score1 > score2) {
            match.winner = match.player1ID;
            match.loser = match.player2ID;
        } else if (score2 > score1) {
            match.winner = match.player2ID;
            match.loser = match.player1ID;
        } else {
            // Tie-breaker
            if (rand() % 2 == 0) {
                match.winner = match.player1ID;
                match.loser = match.player2ID;
            } else {
                match.winner = match.player2ID;
                match.loser = match.player1ID;
            }
        }
        
        // Update player records
        int winnerIndex = Utils::findPlayerIndex(players, playerCount, match.winner);
        int loserIndex = Utils::findPlayerIndex(players, playerCount, match.loser);
        
        if (winnerIndex != -1) players[winnerIndex].wins++;
        if (loserIndex != -1) players[loserIndex].losses++;
        
        cout << "\nMatch Result Simulated!" << endl;
        cout << "Match ID: " << match.matchID << endl;
        cout << "Score: " << score1 << " - " << score2 << endl;
        cout << "Winner: " << match.winner << endl;
        
        // Add to history stack
        matchHistory.push(match.matchID[1]);
    }
    
    void viewTournamentBracket() {
        cout << "\n" << string(60, '=') << endl;
        cout << "              TOURNAMENT BRACKET" << endl;
        cout << string(60, '=') << endl;
        
        if (matchCount == 0) {
            cout << "No matches scheduled for tournament bracket!" << endl;
            return;
        }
        
        cout << "QUALIFIER STAGE:" << endl;
        cout << string(40, '-') << endl;
        displayMatchesByStage("Qualifier");
        
        cout << "\nGROUP STAGE:" << endl;
        cout << string(40, '-') << endl;
        displayMatchesByStage("Group");
        
        cout << "\nKNOCKOUT STAGE:" << endl;
        cout << string(40, '-') << endl;
        displayMatchesByStage("Knockout");
    }
    
    void displayMatchesByStage(string stage) {
        bool found = false;
        for (int i = 0; i < matchCount; i++) {
            if (matches[i].stage == stage) {
                cout << matches[i].matchID << ": ";
                
                int p1Index = Utils::findPlayerIndex(players, playerCount, matches[i].player1ID);
                int p2Index = Utils::findPlayerIndex(players, playerCount, matches[i].player2ID);
                
                string p1Name = (p1Index != -1) ? players[p1Index].name : "Unknown";
                string p2Name = (p2Index != -1) ? players[p2Index].name : "Unknown";
                
                cout << p1Name << " vs " << p2Name;
                
                if (!matches[i].winner.empty()) {
                    cout << " [Winner: " << matches[i].winner << "]";
                } else {
                    cout << " [Pending]";
                }
                cout << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No matches in this stage." << endl;
        }
    }
    
    void updatePlayerRankings() {
        cout << "\n" << string(60, '=') << endl;
        cout << "            UPDATE PLAYER RANKINGS" << endl;
        cout << string(60, '=') << endl;
        
        if (playerCount == 0) {
            cout << "No players to update!" << endl;
            return;
        }
        
        cout << "Current player rankings (sorted by wins):" << endl;
        cout << string(60, '-') << endl;
        
        // Simple bubble sort by wins (descending)
        for (int i = 0; i < playerCount - 1; i++) {
            for (int j = 0; j < playerCount - i - 1; j++) {
                if (players[j].wins < players[j + 1].wins) {
                    Player temp = players[j];
                    players[j] = players[j + 1];
                    players[j + 1] = temp;
                }
            }
        }
        
        // Update rankings based on position
        for (int i = 0; i < playerCount; i++) {
            players[i].ranking = 1500 + (playerCount - i) * 20 + players[i].wins * 10;
            cout << (i + 1) << ". " << players[i].name 
                 << " - Ranking: " << players[i].ranking 
                 << " (W:" << players[i].wins << " L:" << players[i].losses << ")" << endl;
        }
        
        cout << "\nPlayer rankings updated successfully!" << endl;
    }
};