#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "structs.h"
#include "helper.h"
#include "Stack.h"

using namespace std;

class ResultLogger 
{
private:
    MatchResult results[200];
    int resultCount;
    Stack* matchHistory; // Pointer to match history from MatchScheduler
    Player players[100];
    int playerCount;
    
    void displayAdminMenu() {
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

    void displayPlayerMenu() {
        cout << "\n" << string(60, '=') << endl;
        cout << "    GAME RESULT LOGGING & PERFORMANCE HISTORY" << endl;
        cout << string(60, '=') << endl;
        cout << "1. View All Match Results" << endl;
        cout << "2. Generate Tournament Statistics" << endl;
        cout << "0. Return to Main Menu" << endl;
        cout << string(60, '-') << endl;
        cout << "Enter your choice: ";
    }

    // Function 1: Store & Review Recent Match Results (Stack)
    void storeAndReviewRecentMatches() 
    {
        cout << "\n" << string(60, '=') << endl;
        cout << "    STORE & REVIEW RECENT MATCH RESULTS (STACK)" << endl;
        cout << string(60, '=') << endl;

        cout << "--- RECENT MATCHES STACK DISPLAY ---" << endl;
        
        // Check if we have external match history from MatchScheduler
        if (matchHistory != nullptr && !matchHistory->isEmpty()) 
        {
            cout << "Processing matches from MatchScheduler stack..." << endl;
            cout << "Stack size: " << matchHistory->size() << " matches" << endl;
            
            // Process matches from MatchHistory stack and add to results array
            int savedCount = 0;
            Stack tempStack; // Temporary stack to preserve original

            // Move all items to temp stack while processing
            while (!matchHistory->isEmpty()) 
            {
                string matchData = matchHistory->peek();
                matchHistory->pop();
                tempStack.push(matchData);
                
                // Parse the match data and add to results array
                MatchResult newResult;
                if (FileManager::parseMatchResults(matchData, newResult)) 
                {
                    // Check if this match already exists in results
                    bool exists = false;
                    for (int i = 0; i < resultCount; i++) 
                    {
                        if (results[i].matchID == newResult.matchID) 
                        {
                            exists = true;
                            break;
                        }
                    }
                    
                    // Add new match if it doesn't exist and we have space
                    if (!exists && resultCount < 200) 
                    {
                        results[resultCount] = newResult;
                        resultCount++;
                        savedCount++;
                    }
                }
            }
            
            // Restore original stack
            while (!tempStack.isEmpty()) 
            {
                matchHistory->push(tempStack.peek());
                tempStack.pop();
            }
            
            if (savedCount > 0) 
            {
                // Save to CSV file
                FileManager::saveMatchResults(results, resultCount);
                cout << savedCount << " recent result records saved from stack!" << endl;
            } else {
                cout << "Stack is empty or match result records are up-to-date!" << endl;
            }
            
        } else {
            cout << "No external match history available or stack is empty." << endl;
        }
        
        cout << string(50, '-') << endl;
        
        // Display stack contents using the external matchHistory stack
        if (matchHistory != nullptr && !matchHistory->isEmpty()) 
        {
            cout << "\nMatch history stack size: " << matchHistory->size() << " matches" << endl;
            matchHistory->display(); 
        } else {
            cout << "No match history stack available." << endl;
        }
        
        cout << "\n--- DETAILED RECENT MATCH RESULTS (STACK) ---" << endl;
        int displayCount = (resultCount > 5) ? 5 : resultCount;
        if (displayCount > 0) {
            for (int i = resultCount - displayCount; i < resultCount && i >= 0; i++) 
            {
                
                cout << string(45, '-') << endl;
                cout << "    Match ID : " << results[i].matchID << endl;
                cout << "    Winner   : " << results[i].winner << endl;
                cout << "    Loser    : " << results[i].loser << endl;
                cout << "    Score    : " << results[i].team1Score << "-" << results[i].team2Score << endl;
                cout << "    Stage    : " << results[i].stage << endl;
                cout << "    Date     : " << results[i].matchDate << endl;
                cout << "    Duration : " << results[i].duration << endl;
            }
        } else {
            cout << "No detailed match results available." << endl;
        }     
    }

    // Function 2: View All Match Results
    void viewAllResults() 
    {
        cout << "\n=== ALL MATCH RESULTS ===" << endl;
        
        // Load latest data from CSV
        FileManager::loadMatchResults(results, resultCount);
        
        if (resultCount == 0) 
        {
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
        
        for (int i = 0; i < resultCount; i++) 
        {
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
    
    // Function 3: View Player Performance History 
    void viewPlayerPerformance() 
    {
        cout << "\n=== PLAYER PERFORMANCE HISTORY ===" << endl;
        
        if (playerCount == 0) {
            cout << "No player data found." << endl;
            return;
        }
        
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
        
        for (int i = 0; i < playerCount; i++) 
        {
            float winRate = 0.0;
            int totalMatches = players[i].wins + players[i].losses;
            if (totalMatches > 0) 
            {
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
        
        cin.ignore(1000, '\n');
        
        string choice;
        bool validInput = false;
        
        while (!validInput) 
        {
            cout << "\nWould you like to view detailed information for a specific player? (y/n): ";
            getline(cin, choice);
            
            for (int i = 0; i < choice.length(); i++) 
            {
                choice[i] = tolower(choice[i]);
            }
                        
            // Check if input is valid
            if (choice == "y" || choice == "yes" || choice == "n" || choice == "no") 
            {
                validInput = true;
                
                // Process the valid choice
                if (choice == "y" || choice == "yes") 
                {
                    viewSpecificPlayerDetails();
                }
            } else {
                cout << "\nInvalid input! Please enter 'y' (yes) or 'n' (no)." << endl;
                cout << "You entered: '" << choice << "'" << endl;
                cout << "Please try again..." << endl;
            }
        }
    }
    
    void viewSpecificPlayerDetails() 
    {
        string searchID;
        cout << "\nEnter Player ID to view details: ";
        getline(cin, searchID);
        
        // Find the player
        int playerIndex = -1;
        for (int i = 0; i < playerCount; i++) 
        {
            if (players[i].playerID == searchID) 
            {
                playerIndex = i;
                break;
            }
        }
        
        if (playerIndex == -1) 
        {
            cout << "Player with ID '" << searchID << "' not found!" << endl;
            cout << "\nAvailable Player IDs:" << endl;
            for (int i = 0; i < playerCount; i++) 
            {
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
        cout << "Name            : " << player.name << endl;
        cout << "University      : " << player.university << endl;
        cout << "Team            : " << player.team << endl;
        cout << "Current Ranking : " << player.ranking << endl;
        
        // Calculate performance statistics
        int totalMatches = player.wins + player.losses;
        float winRate = (totalMatches > 0) ? (float)player.wins / totalMatches * 100 : 0;
        
        cout << "\n" << string(30, '-') << " STATISTICS " << string(30, '-') << endl;
        cout << "Total Matches   : " << totalMatches << endl;
        cout << "Wins            : " << player.wins << endl;
        cout << "Losses          : " << player.losses << endl;
        cout << "Win Rate        : " << fixed << setprecision(1) << winRate << "%" << endl;
        
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
        
        // Get the player's team
        string playerTeam = player.team;

        for (int i = 0; i < resultCount; i++) 
        {
            bool isPlayerTeamInMatch = false;
            string opponent = "";
            string result = "";
            string score = to_string(results[i].team1Score) + "-" + to_string(results[i].team2Score);
            string actualScore = score; // Keep original score format
            
            // Check if this player's team participated in this match
            if (results[i].team1ID == playerTeam) 
            {
                isPlayerTeamInMatch = true;
                opponent = results[i].team2ID;
                
                // Determine if player's team won or lost
                if (results[i].winner == playerTeam) 
                {
                    result = "WIN";
                } else {
                    result = "LOSS";
                    // For losses, might want to show score as opponent-team format
                    actualScore = to_string(results[i].team2Score) + "-" + to_string(results[i].team1Score);
                }
            } 
            else if (results[i].team2ID == playerTeam) 
            {
                isPlayerTeamInMatch = true;
                opponent = results[i].team1ID;
                
                // Determine if player's team won or lost
                if (results[i].winner == playerTeam) 
                {
                    result = "WIN";
                    // For wins as team2, show score as team2-team1 format
                    actualScore = to_string(results[i].team2Score) + "-" + to_string(results[i].team1Score);
                } else {
                    result = "LOSS";
                }
            }
            
            if (isPlayerTeamInMatch) 
            {
                hasMatches = true;
                cout << left << setw(10) << results[i].matchID
                    << setw(15) << opponent.substr(0, 14)
                    << setw(10) << result
                    << setw(10) << actualScore
                    << setw(15) << results[i].stage.substr(0, 14)
                    << setw(12) << results[i].matchDate << endl;
            }
        }

        if (!hasMatches) 
        {
            cout << "No match history found for this player's team (" << playerTeam << ")." << endl;
        }
    }

    // Function 4: Generate Tournament Statistics - CORRECTED VERSION
    void generateTournamentStats() 
    {
        cout << string(45, '=') << endl;
        cout << "    COMPREHENSIVE TOURNAMENT STATISTICS    " << endl;
        cout << string(45, '=') << endl;

        cout << "=== OVERALL TOURNAMENT SUMMARY ===" << endl;
        cout << "Total Matches Played: " << resultCount << endl;
        cout << "Total Players Registered: " << playerCount << endl;
        
        // Use external matchHistory stack instead of recentMatches
        int stackSize = (matchHistory != nullptr) ? matchHistory->size() : 0;
        cout << "Recent Matches in Stack: " << stackSize << endl;
        
        // 1. Match Distribution by Stage - CORRECTED
        int qualifierMatches = 0, groupMatches = 0, knockoutMatches = 0, otherMatches = 0;
        
        for (int i = 0; i < resultCount; i++) 
        {
            if (results[i].stage == "Qualifiers") qualifierMatches++;  // Note: CSV shows "Qualifiers" not "Qualifier"
            else if (results[i].stage == "Group Stage") groupMatches++;
            else if (results[i].stage == "Knockout Stage") knockoutMatches++;
            else otherMatches++;
        }
        
        cout << "\n=== MATCH DISTRIBUTION BY STAGE ===" << endl;
        if (resultCount > 0) 
        {
            cout << "Qualifier matches: " << qualifierMatches << " (" << fixed << setprecision(1) << (float)qualifierMatches/resultCount*100 << "%)" << endl;
            cout << "Group Stage matches: " << groupMatches << " (" << (float)groupMatches/resultCount*100 << "%)" << endl;
            cout << "Knockout Stage matches: " << knockoutMatches << " (" << (float)knockoutMatches/resultCount*100 << "%)" << endl;
            if (otherMatches > 0) 
            {
                cout << "Other matches: " << otherMatches << " (" << (float)otherMatches/resultCount*100 << "%)" << endl;
            }
        }
        
        // 2. Score Statistics - ENHANCED
        if (resultCount > 0) 
        {
            float totalScore1 = 0, totalScore2 = 0;
            int maxScore = 0, minScore = 999;
            int closestMatch = 999;
            string closestMatchID = "";
            string highestScoringMatchID = "";
            
            for (int i = 0; i < resultCount; i++) 
            {
                totalScore1 += results[i].team1Score;
                totalScore2 += results[i].team2Score;
                
                int maxMatchScore = (results[i].team1Score > results[i].team2Score) ? results[i].team1Score : results[i].team2Score;
                int minMatchScore = (results[i].team1Score < results[i].team2Score) ? results[i].team1Score : results[i].team2Score;
                int scoreDiff = maxMatchScore - minMatchScore;
                int totalMatchScore = results[i].team1Score + results[i].team2Score;
                
                if (maxMatchScore > maxScore) 
                {
                    maxScore = maxMatchScore;
                    highestScoringMatchID = results[i].matchID;
                }
                if (minMatchScore < minScore && minMatchScore >= 0) minScore = minMatchScore;
                if (scoreDiff < closestMatch) 
                {
                    closestMatch = scoreDiff;
                    closestMatchID = results[i].matchID;
                }
            }
            
            cout << "\n=== SCORE STATISTICS ===" << endl;
            cout << "Average Team 1 Score: " << fixed << setprecision(1) << totalScore1/resultCount << endl;
            cout << "Average Team 2 Score: " << totalScore2/resultCount << endl;
            cout << "Average Total Score per Match: " << (totalScore1 + totalScore2)/resultCount << endl;
            cout << "Highest Individual Score: " << maxScore << " (Match: " << highestScoringMatchID << ")" << endl;
            cout << "Lowest Individual Score: " << minScore << endl;
            cout << "Closest Match Score Difference: " << closestMatch << " (Match: " << closestMatchID << ")" << endl;
        }
        
        // 3. Team Performance Analysis - NEW SECTION
        cout << "\n=== TEAM PERFORMANCE ANALYSIS ===" << endl;
        
        // Count team wins and analyze performance
        string teams[50];
        int teamWins[50];
        int teamLosses[50];
        int teamCount = 0;
        
        for (int i = 0; i < resultCount; i++) 
        {
            // Process winner
            bool foundWinner = false;
            for (int j = 0; j < teamCount; j++) 
            {
                if (teams[j] == results[i].winner) 
                {
                    teamWins[j]++;
                    foundWinner = true;
                    break;
                }
            }
            if (!foundWinner && teamCount < 50) 
            {
                teams[teamCount] = results[i].winner;
                teamWins[teamCount] = 1;
                teamLosses[teamCount] = 0;
                teamCount++;
            }
            
            // Process loser
            bool foundLoser = false;
            for (int j = 0; j < teamCount; j++) 
            {
                if (teams[j] == results[i].loser) 
                {
                    teamLosses[j]++;
                    foundLoser = true;
                    break;
                }
            }
            if (!foundLoser && teamCount < 50) 
            {
                teams[teamCount] = results[i].loser;
                teamWins[teamCount] = 0;
                teamLosses[teamCount] = 1;
                teamCount++;
            }
        }
        
        // Sort teams by wins (bubble sort)
        for (int i = 0; i < teamCount - 1; i++) 
        {
            for (int j = 0; j < teamCount - i - 1; j++) 
            {
                if (teamWins[j] < teamWins[j + 1])
                {
                    // Swap wins
                    int tempWins = teamWins[j];
                    teamWins[j] = teamWins[j + 1];
                    teamWins[j + 1] = tempWins;
                    
                    // Swap losses
                    int tempLosses = teamLosses[j];
                    teamLosses[j] = teamLosses[j + 1];
                    teamLosses[j + 1] = tempLosses;
                    
                    // Swap team names
                    string tempName = teams[j];
                    teams[j] = teams[j + 1];
                    teams[j + 1] = tempName;
                }
            }
        }
        
        cout << left << setw(20) << "Team Name" 
            << setw(8) << "Wins" 
            << setw(8) << "Losses" 
            << setw(12) << "Win Rate" << endl;
        cout << string(50, '-') << endl;
        
        for (int i = 0; i < teamCount; i++) 
        {
            int totalMatches = teamWins[i] + teamLosses[i];
            float winRate = (totalMatches > 0) ? (float)teamWins[i] / totalMatches * 100 : 0;
            
            cout << left << setw(20) << teams[i].substr(0, 19)
                << setw(8) << teamWins[i]
                << setw(8) << teamLosses[i]
                << setw(12) << fixed << setprecision(1) << winRate << "%" << endl;
        }

        // 5. Match Duration Analysis - NEW SECTION
        cout << "\n=== MATCH DURATION ANALYSIS ===" << endl;
        
        if (resultCount > 0) 
        {
            string longestMatchID = "";
            string shortestMatchID = "";
            int longestDuration = 0;
            int shortestDuration = 99999;
            int totalMinutes = 0;
            
            for (int i = 0; i < resultCount; i++) 
            {
                // Parse duration (format: MM:SS)
                string duration = results[i].duration;
                int minutes = 0, seconds = 0;
                
                size_t colonPos = duration.find(':');
                if (colonPos != string::npos) 
                {
                    minutes = stoi(duration.substr(0, colonPos));
                    seconds = stoi(duration.substr(colonPos + 1));
                    int totalSeconds = minutes * 60 + seconds;
                    totalMinutes += minutes;
                    
                    if (totalSeconds > longestDuration) 
                    {
                        longestDuration = totalSeconds;
                        longestMatchID = results[i].matchID;
                    }
                    if (totalSeconds < shortestDuration) 
                    {
                        shortestDuration = totalSeconds;
                        shortestMatchID = results[i].matchID;
                    }
                }
            }
            
            cout << "Average Match Duration: " << fixed << setprecision(1) << (float)totalMinutes/resultCount << " minutes" << endl;
            cout << "Longest Match: " << longestDuration/60 << ":" << setfill('0') << setw(2) << longestDuration%60 << " (Match: " << longestMatchID << ")" << endl;
            cout << "Shortest Match: " << shortestDuration/60 << ":" << setfill('0') << setw(2) << shortestDuration%60 << " (Match: " << shortestMatchID << ")" << endl;
            cout << setfill(' ');
        }
        
        // 6. Prize Pool Distribution
        cout << "\n=== PRIZE POOL DISTRIBUTION ===" << endl;
        
        if (teamCount >= 3) 
        {
            double totalPrizePool = 10000.0;
            double firstPrize = totalPrizePool * 0.50;
            double secondPrize = totalPrizePool * 0.30;
            double thirdPrize = totalPrizePool * 0.20;
            
            cout << "Total Prize Pool: $" << fixed << setprecision(2) << totalPrizePool << endl;
            cout << string(50, '-') << endl;
            cout << "PRIZE DISTRIBUTION (Based on Match Wins):" << endl;
            
            string places[] = {"1st", "2nd", "3rd"};
            double prizes[] = {firstPrize, secondPrize, thirdPrize};
            
            for (int i = 0; i < 3 && i < teamCount; i++) 
            {
                cout << places[i] << " Place: " << teams[i] << " - $" 
                    << fixed << setprecision(2) << prizes[i] << endl;
            }
            
            cout << "\n=== PERFORMANCE BONUSES ===" << endl;
            double bonusPool = 2000.0;
            
            for (int i = 0; i < 3 && i < teamCount; i++) 
            {
                double performanceBonus = 0;
                if (teamWins[i] >= 3) 
                {
                    performanceBonus = bonusPool * 0.50;
                } else if (teamWins[i] >= 2) {
                    performanceBonus = bonusPool * 0.30;
                } else if (teamWins[i] >= 1) {
                    performanceBonus = bonusPool * 0.20;
                }
                
                if (performanceBonus > 0)
                {
                    cout << teams[i] << " - Performance Bonus: $" << fixed << setprecision(2) 
                        << performanceBonus << " (for " << teamWins[i] << " wins)" << endl;
                }
            }
            
        } else {
            cout << "Insufficient teams for full prize distribution." << endl;
        }
        
        cout << "\n" << string(60, '=') << endl;
        cout << "    STACK-BASED TOURNAMENT ANALYSIS COMPLETE" << endl;
        cout << string(60, '=') << endl;
    }

public:
    ResultLogger() : matchHistory(nullptr) 
    {
        resultCount = 0;
        playerCount = 0;
        
        // Load data from files
        FileManager::loadPlayers(players, playerCount);
        FileManager::loadMatchResults(results, resultCount);
    }
    
    ~ResultLogger() 
    {
        // Save data when exiting
        FileManager::savePlayers(players, playerCount);
        FileManager::saveMatchResults(results, resultCount);
    }

    // Method to set external match history stack
    void setStack(Stack* mh) 
    {
        matchHistory = mh;
    }
    
    void runPlayer() 
    {
        int choice;
        
        do 
        {
            displayPlayerMenu();
            cin >> choice;
            
            if (cin.fail()) 
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            
            switch (choice) 
            {
                case 1:
                    viewAllResults();
                    break;
                case 2:
                    generateTournamentStats();
                    break;
                case 0:
                    cout << "\nReturning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please select 0-4." << endl;
                    break;
            }
            
            if (choice != 0) 
            {
                Utils::pauseScreen();
            }
            
        } while (choice != 0);
    }

    void runAdmin() 
    {
        int choice;
        
        do 
        {
            displayAdminMenu();
            cin >> choice;
            
            if (cin.fail()) 
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            
            switch (choice) 
            {
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
            
            if (choice != 0) 
            {
                Utils::pauseScreen();
            }
            
        } while (choice != 0);
    }
};