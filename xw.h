#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "structs.h"
#include "helper.h"
#include "Stack.h"

using namespace std;

class ResultLogger {
private:
    MatchResult results[200];
    int resultCount;
    Stack recentMatches;
    TournamentRecord tournaments[50];
    int tournamentCount;
    Player players[100];
    int playerCount;
    
    void displayMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    GAME RESULT LOGGING & PERFORMANCE HISTORY" << endl;
        cout << string(60, '=') << endl;
        cout << "1. View Recent Match Results (Stack)" << endl;
        cout << "2. View All Match Results" << endl;
        cout << "3. View Player Performance History" << endl;
        cout << "4. Generate Tournament Statistics" << endl;
        cout << "5. Display Tournament Records" << endl;
        cout << "6. Save Tournament Records to File" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter your choice: ";
    }
       
    void updatePlayerStats(string player1, string player2, string winner, int score1, int score2) {
        // Find and update player statistics
        for (int i = 0; i < playerCount; i++) {
            if (players[i].name == player1 || players[i].playerID == player1) {
                if (winner == player1) {
                    players[i].wins++;
                } else {
                    players[i].losses++;
                }
            }
            if (players[i].name == player2 || players[i].playerID == player2) {
                if (winner == player2) {
                    players[i].wins++;
                } else {
                    players[i].losses++;
                }
            }
        }
    }
    
    void viewRecentMatches() {
        cout << "\n=== RECENT MATCH RESULTS (STACK) ===" << endl;
        
        if (recentMatches.isEmpty()) {
            cout << "No recent matches found." << endl;
            return;
        }
        
        cout << "Recent matches (most recent first):" << endl;
        recentMatches.display();
        
        // Show details of recent matches
        cout << "\nDetailed Recent Match Results:" << endl;
        cout << string(80, '-') << endl;
        cout << left << setw(10) << "Match ID" 
             << setw(15) << "Player 1" 
             << setw(15) << "Player 2" 
             << setw(15) << "Winner" 
             << setw(10) << "Score" 
             << setw(15) << "Stage" << endl;
        cout << string(80, '-') << endl;
        
        // Display last 5 matches
        int displayCount = (resultCount > 5) ? 5 : resultCount;
        for (int i = resultCount - 1; i >= resultCount - displayCount; i--) {
            cout << left << setw(10) << results[i].matchID
                 << setw(15) << results[i].player1.substr(0, 14)
                 << setw(15) << results[i].player2.substr(0, 14)
                 << setw(15) << results[i].winner.substr(0, 14)
                 << setw(10) << (to_string(results[i].score1) + "-" + to_string(results[i].score2))
                 << setw(15) << results[i].stage << endl;
        }
    }
    
    void viewAllResults() {
        cout << "\n=== ALL MATCH RESULTS ===" << endl;
        
        if (resultCount == 0) {
            cout << "No match results found." << endl;
            return;
        }
        
        cout << "Total matches: " << resultCount << endl;
        cout << string(100, '-') << endl;
        cout << left << setw(10) << "Match ID" 
             << setw(15) << "Player 1" 
             << setw(15) << "Player 2" 
             << setw(15) << "Winner" 
             << setw(10) << "Score" 
             << setw(12) << "Stage"
             << setw(20) << "Timestamp" << endl;
        cout << string(100, '-') << endl;
        
        for (int i = 0; i < resultCount; i++) {
            cout << left << setw(10) << results[i].matchID
                 << setw(15) << results[i].player1.substr(0, 14)
                 << setw(15) << results[i].player2.substr(0, 14)
                 << setw(15) << results[i].winner.substr(0, 14)
                 << setw(10) << (to_string(results[i].score1) + "-" + to_string(results[i].score2))
                 << setw(12) << results[i].stage
                 << setw(20) << results[i].timestamp.substr(0, 19) << endl;
        }
    }
    
    void viewPlayerPerformance() {
        cout << "\n=== PLAYER PERFORMANCE HISTORY ===" << endl;
        
        if (playerCount == 0) {
            cout << "No player data found." << endl;
            return;
        }
        
        cout << string(90, '-') << endl;
        cout << left << setw(8) << "ID" 
             << setw(20) << "Name" 
             << setw(25) << "University"
             << setw(8) << "Wins" 
             << setw(8) << "Losses"
             << setw(8) << "Ranking"
             << setw(12) << "Win Rate" << endl;
        cout << string(90, '-') << endl;
        
        for (int i = 0; i < playerCount; i++) {
            float winRate = 0.0;
            int totalMatches = players[i].wins + players[i].losses;
            if (totalMatches > 0) {
                winRate = (float)players[i].wins / totalMatches * 100;
            }
            
            cout << left << setw(8) << players[i].playerID
                 << setw(20) << players[i].name.substr(0, 19)
                 << setw(25) << players[i].university.substr(0, 24)
                 << setw(8) << players[i].wins
                 << setw(8) << players[i].losses
                 << setw(8) << players[i].ranking
                 << setw(12) << fixed << setprecision(1) << winRate << "%" << endl;
        }
    }
    
    void generateTournamentStats() {
        cout << "\n=== TOURNAMENT STATISTICS ===" << endl;
        
        if (resultCount == 0) {
            cout << "No match data available for statistics." << endl;
            return;
        }
        
        // Count matches by stage
        int qualifierMatches = 0, groupMatches = 0, knockoutMatches = 0, finalMatches = 0, wildcardMatches = 0;
        
        for (int i = 0; i < resultCount; i++) {
            if (results[i].stage == "Qualifier") qualifierMatches++;
            else if (results[i].stage == "Group Stage") groupMatches++;
            else if (results[i].stage == "Semi-Final") knockoutMatches++;
            else if (results[i].stage == "Final") finalMatches++;
            else if (results[i].stage == "Wildcard") wildcardMatches++;
        }
        
        cout << "Match Distribution by Stage:" << endl;
        cout << "- Qualifier matches: " << qualifierMatches << endl;
        cout << "- Group Stage matches: " << groupMatches << endl;
        cout << "- Semi-Final matches: " << knockoutMatches << endl;
        cout << "- Final matches: " << finalMatches << endl;
        cout << "- Wildcard matches: " << wildcardMatches << endl;
        cout << "- Total matches: " << resultCount << endl;
        
        // Find most active players
        cout << "\nMost Active Players (by total matches played):" << endl;
        for (int i = 0; i < playerCount && i < 5; i++) {
            int totalMatches = players[i].wins + players[i].losses;
            if (totalMatches > 0) {
                cout << (i + 1) << ". " << players[i].name 
                     << " (" << totalMatches << " matches)" << endl;
            }
        }
        
        // Average scores
        if (resultCount > 0) {
            float avgScore1 = 0, avgScore2 = 0;
            for (int i = 0; i < resultCount; i++) {
                avgScore1 += results[i].score1;
                avgScore2 += results[i].score2;
            }
            avgScore1 /= resultCount;
            avgScore2 /= resultCount;
            
            cout << "\nAverage Match Scores:" << endl;
            cout << "- Average Winner score: " << fixed << setprecision(1) << avgScore1 << endl;
            cout << "- Average Runner-up score: " << fixed << setprecision(1) << avgScore2 << endl;
        }
    }
    
    void saveResultsToFile() {
        cout << "\n=== SAVE TOURNAMENT RECORDS TO FILE ===" << endl;
        
        ofstream file("Tournament_Record.csv");
        if (!file.is_open()) {
            cout << "Error: Cannot create Tournament_Record.csv file!" << endl;
            return;
        }
        
        // Write header to match the new tournament record structure
        file << "RecordID,TournamentName,Date,TournamentStage,Winner,RunnerUp,WinnerScore,RunnerUpScore,TotalParticipants,Status\n";
        
        // Write all tournament records
        for (int i = 0; i < tournamentCount; i++) {
            file << tournaments[i].recordID << ","
                 << tournaments[i].tournamentName << ","
                 << tournaments[i].date << ","
                 << tournaments[i].tournamentStage << ","
                 << tournaments[i].winner << ","
                 << tournaments[i].runnerUp << ","
                 << tournaments[i].winnerScore << ","
                 << tournaments[i].runnerUpScore << ","
                 << tournaments[i].totalParticipants << ","
                 << tournaments[i].status << "\n";
        }
        
        file.close();
        cout << "Successfully saved " << tournamentCount << " tournament records to Tournament_Record.csv" << endl;
    }
    
    void displayTournamentRecords() {
        cout << "\n=== TOURNAMENT RECORDS ===" << endl;
        
        if (tournamentCount == 0) {
            cout << "No tournament records found." << endl;
            return;
        }
        
        cout << string(120, '-') << endl;
        cout << left << setw(6) << "ID" 
             << setw(25) << "Tournament Name" 
             << setw(12) << "Date"
             << setw(15) << "Stage"
             << setw(15) << "Winner" 
             << setw(15) << "Runner-up"
             << setw(8) << "W-Score"
             << setw(8) << "R-Score"
             << setw(8) << "Total"
             << setw(12) << "Status" << endl;
        cout << string(120, '-') << endl;
        
        for (int i = 0; i < tournamentCount; i++) {
            cout << left << setw(6) << tournaments[i].recordID
                 << setw(25) << tournaments[i].tournamentName.substr(0, 24)
                 << setw(12) << tournaments[i].date
                 << setw(15) << tournaments[i].tournamentStage.substr(0, 14)
                 << setw(15) << tournaments[i].winner.substr(0, 14)
                 << setw(15) << tournaments[i].runnerUp.substr(0, 14)
                 << setw(8) << tournaments[i].winnerScore
                 << setw(8) << tournaments[i].runnerUpScore
                 << setw(8) << tournaments[i].totalParticipants
                 << setw(12) << tournaments[i].status << endl;
        }
    }

public:
    ResultLogger() {
        resultCount = 0;
        tournamentCount = 0;
        playerCount = 0;
        
        // Load data from files
        FileManager::loadPlayers(players, playerCount);
        FileManager::loadTournamentRecords(tournaments, tournamentCount);
    }
    
    ~ResultLogger() {
        // Save data when exiting
        FileManager::savePlayers(players, playerCount);
        FileManager::saveTournamentRecords(tournaments, tournamentCount);
        saveResultsToFile();
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
                    viewRecentMatches();
                    break;
                case 2:
                    viewAllResults();
                    break;
                case 3:
                    viewPlayerPerformance();
                    break;
                case 4:
                    generateTournamentStats();
                    break;
                case 5:
                    displayTournamentRecords();
                    break;
                case 6:
                    saveResultsToFile();
                    break;
                case 0:
                    cout << "\nReturning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please select 0-6." << endl;
                    break;
            }
            
            if (choice != 0) {
                Utils::pauseScreen();
            }
            
        } while (choice != 0);
    }
};