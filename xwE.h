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
    Player players[100];
    int playerCount;
    
    void displayMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    GAME RESULT LOGGING & PERFORMANCE HISTORY" << endl;
        cout << string(60, '=') << endl;
        cout << "1. Store & Review Recent Match Results (Stack)" << endl;
        cout << "2. View All Match Results" << endl;
        cout << "3. View Player Performance History" << endl;
        cout << "4. Generate Tournament Statistics" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter your choice: ";
    }
    
    // Function 1: Store & Review Recent Match Results (Stack)
    void storeAndReviewRecentMatches() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    STORE & REVIEW RECENT MATCH RESULTS (STACK)" << endl;
        cout << string(60, '=') << endl;
    
        cout << "--- RECENT MATCHES STACK DISPLAY ---" << endl;
        
        if (recentMatches.isEmpty()) {
            cout << "No recent matches in stack!" << endl;
        } else {
            cout << "Stack size: " << recentMatches.size() << " matches" << endl;
            cout << "Stack contents using Stack.display() method:" << endl;
            // Display match results contents using Stack class method
            recentMatches.display(); 
        }
        cout << string(50, '-') << endl;
        
        // Automatically save all stack results to CSV
        FileManager::saveMatchResults(results, resultCount);
        
        // Display detailed recent match results in tabular format
        cout << "\n--- DETAILED RECENT MATCH RESULTS (STACK) ---" << endl;
        cout << string(110, '-') << endl;
        cout << left << setw(9) << "Match ID" 
             << setw(15) << "Team 1" 
             << setw(15) << "Team 2" 
             << setw(15) << "Winner" 
             << setw(8) << "Score" 
             << setw(15) << "Stage"
             << setw(12) << "Date"
             << setw(10) << "Duration" << endl;
        cout << string(110, '-') << endl;
        
        // Display last 5 matches (most recent)
        int displayCount = (resultCount > 5) ? 5 : resultCount;
        if (displayCount > 0) {
            for (int i = resultCount - 1; i >= resultCount - displayCount && i >= 0; i--) {
                cout << left << setw(9) << results[i].matchID
                     << setw(15) << results[i].team1ID.substr(0, 14)
                     << setw(15) << results[i].team2ID.substr(0, 14)
                     << setw(15) << results[i].winner.substr(0, 14)
                     << setw(8) << (to_string(results[i].team1Score) + "-" + to_string(results[i].team2Score))
                     << setw(15) << results[i].stage.substr(0, 14)
                     << setw(12) << results[i].matchDate
                     << setw(10) << results[i].duration << endl;
            }
        } else {
            cout << "No detailed match results available." << endl;
        }
        cout << string(110, '=') << endl;
    }

    // Function 2: View All Match Results
    void viewAllResults() {
        cout << "\n=== ALL MATCH RESULTS ===" << endl;
        
        // Load latest data from CSV
        FileManager::loadMatchResults(results, resultCount);
        
        if (resultCount == 0) {
            cout << "No match results found." << endl;
            return;
        }
        
        cout << "Total matches: " << resultCount << endl;
        cout << string(120, '-') << endl;
        cout << left << setw(9) << "Match ID" 
             << setw(15) << "Team 1" 
             << setw(15) << "Team 2" 
             << setw(15) << "Winner" 
             << setw(15) << "Loser"
             << setw(8) << "Score" 
             << setw(15) << "Stage"
             << setw(12) << "Date"
             << setw(10) << "Duration" << endl;
        cout << string(120, '-') << endl;
        
        for (int i = 0; i < resultCount; i++) {
            cout << left << setw(8) << results[i].matchID
                 << setw(15) << results[i].team1ID.substr(0, 15)
                 << setw(15) << results[i].team2ID.substr(0, 15)
                 << setw(15) << results[i].winner.substr(0, 15)
                 << setw(15) << results[i].loser.substr(0, 15)
                 << setw(8) << (to_string(results[i].team1Score) + "-" + to_string(results[i].team2Score))
                 << setw(15) << results[i].stage.substr(0, 14)
                 << setw(12) << results[i].matchDate
                 << setw(10) << results[i].duration << endl;
        }
        cout << string(120, '=') << endl; 
    }
    
// Function 3: View Player Performance History - FIXED VERSION
    void viewPlayerPerformance() {
        cout << "\n=== PLAYER PERFORMANCE HISTORY ===" << endl;
        
        if (playerCount == 0) {
            cout << "No player data found." << endl;
            return;
        }
        
        // Update player stats based on match results
        updatePlayerStatsFromMatches();
        
        cout << string(110, '-') << endl;
        cout << left << setw(8) << "ID" 
             << setw(20) << "Name" 
             << setw(25) << "University"
             << setw(15) << "Team"
             << setw(8) << "Wins" 
             << setw(8) << "Losses"
             << setw(8) << "Ranking"
             << setw(12) << "Win Rate" << endl;
        cout << string(110, '-') << endl;
        
        for (int i = 0; i < playerCount; i++) {
            float winRate = 0.0;
            int totalMatches = players[i].wins + players[i].losses;
            if (totalMatches > 0) {
                winRate = (float)players[i].wins / totalMatches * 100;
            }
            
            cout << left << setw(8) << players[i].playerID
                 << setw(20) << players[i].name.substr(0, 19)
                 << setw(25) << players[i].university.substr(0, 24)
                 << setw(15) << players[i].team.substr(0, 14)
                 << setw(8) << players[i].wins
                 << setw(8) << players[i].losses
                 << setw(8) << players[i].ranking
                 << setw(12) << fixed << setprecision(1) << winRate << "%" << endl;
        }
        cout << string(110, '=') << endl;
        
        // FIX: Clear input buffer and properly handle user input
        cin.ignore(1000, '\n'); // Clear any remaining input in buffer
        
        cout << "\nWould you like to view detailed information for a specific player? (y/n): ";
        string choice;
        getline(cin, choice); // Use getline instead of cin >> to avoid buffer issues
        
        if (choice == "y" || choice == "Y" || choice == "yes" || choice == "Yes" || choice == "YES") {
            viewSpecificPlayerDetails();
        }
    }
    
    void viewSpecificPlayerDetails() {
        string searchID;
        cout << "\nEnter Player ID to view details: ";
        getline(cin, searchID); // Use getline for consistent input handling
        
        // Find the player
        int playerIndex = -1;
        for (int i = 0; i < playerCount; i++) {
            if (players[i].playerID == searchID) {
                playerIndex = i;
                break;
            }
        }
        
        if (playerIndex == -1) {
            cout << "Player with ID '" << searchID << "' not found!" << endl;
            cout << "\nAvailable Player IDs:" << endl;
            for (int i = 0; i < playerCount; i++) {
                cout << "- " << players[i].playerID << " (" << players[i].name << ")" << endl;
            }
            return;
        }
        
        // Display detailed player information
        Player& player = players[playerIndex];
        cout << "\n" << string(80, '=') << endl;
        cout << "           DETAILED PLAYER PROFILE" << endl;
        cout << string(80, '=') << endl;
        
        cout << "Player ID       : " << player.playerID << endl;
        cout << "Name           : " << player.name << endl;
        cout << "University     : " << player.university << endl;
        cout << "Team           : " << player.team << endl;
        cout << "Current Ranking: " << player.ranking << endl;
        
        // Calculate performance statistics
        int totalMatches = player.wins + player.losses;
        float winRate = (totalMatches > 0) ? (float)player.wins / totalMatches * 100 : 0;
        
        cout << "\n" << string(40, '-') << " STATISTICS " << string(40, '-') << endl;
        cout << "Total Matches  : " << totalMatches << endl;
        cout << "Wins           : " << player.wins << endl;
        cout << "Losses         : " << player.losses << endl;
        cout << "Win Rate       : " << fixed << setprecision(1) << winRate << "%" << endl;
        // Find and display match history for this player
        cout << "\n" << string(30, '-') << " MATCH HISTORY " << string(30, '-') << endl;
        
        bool hasMatches = false;
        cout << left << setw(10) << "Match ID" 
             << setw(15) << "Opponent" 
             << setw(10) << "Result" 
             << setw(10) << "Score"
             << setw(15) << "Stage"
             << setw(12) << "Date" << endl;
        cout << string(75, '-') << endl;
        
        for (int i = 0; i < resultCount; i++) {
            bool isPlayerInMatch = false;
            string opponent = "";
            string result = "";
            string score = to_string(results[i].team1Score) + "-" + to_string(results[i].team2Score);
            
            // Check if this player participated in this match
            if (results[i].winner == player.playerID || results[i].winner == player.name) {
                isPlayerInMatch = true;
                result = "WIN";
                // Find opponent (the loser)
                opponent = results[i].loser;
            } else if (results[i].loser == player.playerID || results[i].loser == player.name) {
                isPlayerInMatch = true;
                result = "LOSS";
                // Find opponent (the winner)
                opponent = results[i].winner;
            }
            
            if (isPlayerInMatch) {
                hasMatches = true;
                cout << left << setw(10) << results[i].matchID
                     << setw(15) << opponent.substr(0, 14)
                     << setw(10) << result
                     << setw(10) << score
                     << setw(15) << results[i].stage.substr(0, 14)
                     << setw(12) << results[i].matchDate << endl;
            }
        }
        
        if (!hasMatches) {
            cout << "No match history found for this player." << endl;
        }
    }

    // Function 4: Generate Tournament Statistics - MODIFIED VERSION
    void generateTournamentStats() 
    {
        cout << string(45, '=') << endl;
        cout << "    COMPREHENSIVE TOURNAMENT STATISTICS    " << endl;
        cout << string(45, '=') << endl;

        cout << "=== OVERALL TOURNAMENT SUMMARY ===" << endl;
        cout << "Total Matches Played: " << resultCount << endl;
        cout << "Total Players Registered: " << playerCount << endl;
        cout << "Recent Matches in Stack: " << recentMatches.size() << endl;
        
        // 1. Match Distribution by Stage
        int qualifierMatches = 0, groupMatches = 0, knockoutMatches = 0;
        
        for (int i = 0; i < resultCount; i++) {
            if (results[i].stage == "Qualifier") qualifierMatches++;
            else if (results[i].stage == "Group Stage") groupMatches++;
            else if (results[i].stage == "Knockout Stage") knockoutMatches++;
        }
        
        cout << "\n=== MATCH DISTRIBUTION BY STAGE ===" << endl;
        cout << "Qualifier matches: " << qualifierMatches << " (" << fixed << setprecision(1) << (float)qualifierMatches/resultCount*100 << "%)" << endl;
        cout << "Group Stage matches: " << groupMatches << " (" << (float)groupMatches/resultCount*100 << "%)" << endl;
        cout << "Knockout Stage matches: " << knockoutMatches << " (" << (float)knockoutMatches/resultCount*100 << "%)" << endl;
        
        // 2. Score Statistics
        float totalScore1 = 0, totalScore2 = 0;
        int maxScore = 0, minScore = 999;
        int closestMatch = 999;
        
        for (int i = 0; i < resultCount; i++) {
            totalScore1 += results[i].team1Score;
            totalScore2 += results[i].team2Score;
            
            int maxMatchScore = (results[i].team1Score > results[i].team2Score) ? results[i].team1Score : results[i].team2Score;
            int minMatchScore = (results[i].team1Score < results[i].team2Score) ? results[i].team1Score : results[i].team2Score;
            int scoreDiff = maxMatchScore - minMatchScore;
            
            if (maxMatchScore > maxScore) maxScore = maxMatchScore;
            if (minMatchScore < minScore) minScore = minMatchScore;
            if (scoreDiff < closestMatch) closestMatch = scoreDiff;
        }
        
        cout << "\n=== SCORE STATISTICS ===" << endl;
        cout << "Average Team 1 Score: " << fixed << setprecision(1) << totalScore1/resultCount << endl;
        cout << "Average Team 2 Score: " << totalScore2/resultCount << endl;
        cout << "Highest Score in Tournament: " << maxScore << endl;
        cout << "Lowest Score in Tournament: " << minScore << endl;
        cout << "Closest Match Score Difference: " << closestMatch << endl;
        
        // 3. Stack-Based Recent Match Analysis
        cout << "\n=== RECENT MATCH ANALYSIS (FROM STACK) ===" << endl;
        cout << "Stack Size: " << recentMatches.size() << " matches" << endl;
        
        if (!recentMatches.isEmpty()) {
            cout << "Most Recent Match ID: " << recentMatches.peek() << endl;
            
            // Analyze recent matches performance
            int recentWins = 0, recentLosses = 0;
            for (int i = resultCount - 1; i >= resultCount - recentMatches.size() && i >= 0; i--) {
                // Count different types of results in recent matches
                if (results[i].team1Score > results[i].team2Score) recentWins++;
                else recentLosses++;
            }
            
            cout << "Recent Match Outcomes Analysis:" << endl;
            cout << "- Higher scoring team wins: " << recentWins << endl;
            cout << "- Lower scoring team wins: " << recentLosses << endl;
        } else {
            cout << "No recent matches in stack for analysis." << endl;
        }
        
        // 4. Most Successful Teams/Players
        cout << "\n=== MOST SUCCESSFUL PARTICIPANTS ===" << endl;
        
        // Count wins for each participant
        string winners[200];
        int winCounts[200];
        int uniqueWinners = 0;
        
        for (int i = 0; i < resultCount; i++) {
            bool found = false;
            for (int j = 0; j < uniqueWinners; j++) {
                if (winners[j] == results[i].winner) {
                    winCounts[j]++;
                    found = true;
                    break;
                }
            }
            if (!found && uniqueWinners < 200) {
                winners[uniqueWinners] = results[i].winner;
                winCounts[uniqueWinners] = 1;
                uniqueWinners++;
            }
        }
        
        // Sort by win count (bubble sort)
        for (int i = 0; i < uniqueWinners - 1; i++) {
            for (int j = 0; j < uniqueWinners - i - 1; j++) {
                if (winCounts[j] < winCounts[j + 1]) {
                    // Swap win counts
                    int tempCount = winCounts[j];
                    winCounts[j] = winCounts[j + 1];
                    winCounts[j + 1] = tempCount;
                    
                    // Swap winner names
                    string tempName = winners[j];
                    winners[j] = winners[j + 1];
                    winners[j + 1] = tempName;
                }
            }
        }
        
        cout << "Top 5 Winners:" << endl;
        int topCount = (uniqueWinners > 5) ? 5 : uniqueWinners;
        for (int i = 0; i < topCount; i++) {
            cout << (i + 1) << ". " << winners[i] << " (" << winCounts[i] << " wins)" << endl;
        }

        // 5. Prize Pool Distribution for Top 3
        cout << "\n=== PRIZE POOL DISTRIBUTION ===" << endl;
        
        if (uniqueWinners >= 3) {
            // Define prize pool amounts
            double totalPrizePool = 10000.0; // Total prize pool
            double firstPrize = totalPrizePool * 0.50;  // 50% for 1st place
            double secondPrize = totalPrizePool * 0.30; // 30% for 2nd place
            double thirdPrize = totalPrizePool * 0.20;  // 20% for 3rd place
            
            cout << "Total Prize Pool: $" << fixed << setprecision(2) << totalPrizePool << endl;
            cout << string(50, '-') << endl;
            cout << "PRIZE DISTRIBUTION:" << endl;
            cout << "1st Place: " << left << setw(20) << winners[0] 
                << " - $" << firstPrize << " (" << winCounts[0] << " wins)" << endl;
            cout << "2nd Place: " << left << setw(20) << winners[1] 
                << " - $" << secondPrize << " (" << winCounts[1] << " wins)" << endl;
            cout << "3rd Place: " << left << setw(20) << winners[2] 
                << " - $" << thirdPrize << " (" << winCounts[2] << " wins)" << endl;
            
            // Calculate performance bonus
            cout << "\n=== PERFORMANCE BONUSES ===" << endl;
            double bonusPool = 2000.0; // Additional bonus pool
            
            for (int i = 0; i < 3 && i < uniqueWinners; i++) {
                double performanceBonus = 0;
                if (winCounts[i] >= 5) {
                    performanceBonus = bonusPool * 0.50; // 50% for exceptional performance
                } else if (winCounts[i] >= 3) {
                    performanceBonus = bonusPool * 0.30; // 30% for good performance
                } else if (winCounts[i] >= 2) {
                    performanceBonus = bonusPool * 0.20; // 20% for decent performance
                }
                
                if (performanceBonus > 0) {
                    cout << winners[i] << " - Performance Bonus: $" << fixed << setprecision(2) 
                        << performanceBonus << " (for " << winCounts[i] << " wins)" << endl;
                }
            }
            
        } else {
            cout << "Insufficient participants for full prize distribution." << endl;
            cout << "Need at least 3 winners for complete prize pool allocation." << endl;
        }
        
        cout << "\n" << string(60, '=') << endl;
        cout << "    STACK-BASED TOURNAMENT ANALYSIS COMPLETE" << endl;
        cout << string(60, '=') << endl;
    }
    
    // Helper functions
    void updatePlayerStatsFromMatches() {
        // Reset all player stats
        for (int i = 0; i < playerCount; i++) {
            players[i].wins = 0;
            players[i].losses = 0;
        }
        
        // Update stats from match results
        for (int i = 0; i < resultCount; i++) {
            // Find winner and update wins
            for (int j = 0; j < playerCount; j++) {
                if (players[j].playerID == results[i].winner || players[j].name == results[i].winner) {
                    players[j].wins++;
                }
                if (players[j].playerID == results[i].loser || players[j].name == results[i].loser) {
                    players[j].losses++;
                }
            }
        }
    }

public:
    ResultLogger() {
        resultCount = 0;
        playerCount = 0;
        
        // Load data from files
        FileManager::loadPlayers(players, playerCount);
        FileManager::loadMatchResults(results, resultCount);
        
        // Initialize stack with recent matches with complete details
        for (int i = 0; i < resultCount && i < 10; i++) 
        {
            if (!results[i].matchID.empty()) 
            {
                // Create complete match details string
                string completeDetails = results[i].matchID + "," + 
                                    results[i].team1ID + "," + 
                                    results[i].team2ID + "," + 
                                    results[i].winner + "," + 
                                    results[i].loser + "," + 
                                    results[i].matchDate + "," + 
                                    results[i].stage + "," + 
                                    to_string(results[i].team1Score) + "," + 
                                    to_string(results[i].team2Score) + "," + 
                                    results[i].duration;
                
                recentMatches.push(completeDetails);
            }
        }
    }
    
    ~ResultLogger() {
        // Save data when exiting
        FileManager::savePlayers(players, playerCount);
        FileManager::saveMatchResults(results, resultCount);
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
                    storeAndReviewRecentMatches();
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
                case 0:
                    cout << "\nReturning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please select 0-4." << endl;
                    break;
            }
            
            if (choice != 0) {
                Utils::pauseScreen();
            }
            
        } while (choice != 0);
    }
};