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
    Stack matchHistory;           // Stack to store completed match results with details
    Queue matchQueue;             // Queue to manage pending matches with match IDs
    Player players[100];          // Array to store players
    MatchResult matches[25];      // Array to store matches
    Team teams[8];                // Array to store tournament teams (limited to 8)
    int playerCount;              // Total number of players
    int matchCount;               // Total number of matches
    int teamCount;                // Total number of teams

    // Tournament stage enumeration for tracking current stage
    enum TournamentStage { 
        QUALIFIERS, 
        GROUP_STAGE, 
        KNOCKOUT_STAGE_3RDPLACE,  // 1st variation of Knockout Stage
        KNOCKOUT_STAGE_FINALS,    // 2nd variation of Knockout Stage
        COMPLETED 
    };
    TournamentStage currentStage; // Current stage of the tournament
    
public:
    // Constructor to initialize the match scheduler
    MatchScheduler() {
        playerCount = 0;
        matchCount = 0;
        teamCount = 0;
        currentStage = QUALIFIERS;
        srand(time(0));  // Seed for random operations
        
        // Load players and match results from CSV files
        FileManager::loadPlayers(players, playerCount);
        FileManager::loadMatchResults(matches, matchCount);
        
        organizeTeams();           // Organize players into teams
        updateTeamStandings();     // Update team standings based on matches
        determineCurrentStage();   // Determine the current tournament stage
        
        cout << "Tournament initialized with " << teamCount << " teams." << endl;
    }
    
    // Destructor to save data when the object is destroyed
    ~MatchScheduler() {
        FileManager::savePlayers(players, playerCount);  // Save player data
        FileManager::saveMatchResults(matches, matchCount);  // Save match data
    }

    // Method to get a pointer to the match history stack
    Stack* getStack() {
        return &matchHistory;  // Return reference to match history stack
    }
    
    // Method to run the main task loop for the scheduler
    void runTask() {
        int choice;
        
        do {
            displayMenu();  // Display user menu options
            cout << "Enter your choice: ";
            cin >> choice;
            
            if (cin.fail()) {  // Check for invalid input
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            
            switch (choice) {
                case 1:
                    scheduleMatches();  // Schedule matches for the current stage
                    break;
                case 2:
                    processMatches();  // Process and record match results
                    break;
                case 3:
                    displayPlayerStandings();  // Display current player standings
                    break;
                case 4:
                    displayTournamentBracket();  // Display the tournament bracket
                    break;
                case 0:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    break;
            }
        } while (choice != 0);  // Continue until the user chooses to exit
    }
    
private:
    void waitForEnter() 
    {
        cout << "\nPress Enter to Return...";
        cin.ignore();
        cin.get();
    }

    void displayMenu() {
        cout << "\n" << string(75, '=') << endl;
        cout << "            TASK 1: MATCH SCHEDULING & PLAYER PROGRESSION" << endl;
        cout << string(75, '=') << endl;
        cout << "1. Schedule Matches (Current Stage: " << getStageString(currentStage) << ")" << endl;
        cout << "2. Process Match Results" << endl;
        cout << "3. Display Player Standings" << endl;
        cout << "4. Display Tournament Bracket" << endl;
        cout << "0. Return to Main Menu" << endl;
    }
    
    string getStageString(TournamentStage stage) {
        switch (stage) {
            case QUALIFIERS: return "Qualifiers";
            case GROUP_STAGE: return "Group Stage";
            case KNOCKOUT_STAGE_3RDPLACE: return "Knockout Stage - 3rd Place";
            case KNOCKOUT_STAGE_FINALS: return "Knockout Stage - Final";
            case COMPLETED: return "Tournament Completed";
            default: return "Unknown";
        }
    }
        
    void organizeTeams() {
        // Extract unique teams from players
        string uniqueTeams[50];
        int uniqueCount = 0;
        
        for (int i = 0; i < playerCount; i++) {
            if (players[i].team == "" || players[i].team == "-") continue;
            
            bool exists = false;
            for (int j = 0; j < uniqueCount; j++) {
                if (uniqueTeams[j] == players[i].team) {
                    exists = true;
                    break;
                }
            }
            
            if (!exists && uniqueCount < 50) {
                uniqueTeams[uniqueCount] = players[i].team;
                uniqueCount++;
            }
        }
        
        // Create teams (limit to 8 for tournament)
        teamCount = (uniqueCount > 8) ? 8 : uniqueCount;
        for (int i = 0; i < teamCount; i++) {
            teams[i].teamName = uniqueTeams[i];
            teams[i].playerCount = getTeamPlayerCount(uniqueTeams[i]);
            teams[i].wins = 0;
            teams[i].losses = 0;
        }
    }
    
    int getTeamPlayerCount(string teamName) {
        int count = 0;
        for (int i = 0; i < playerCount; i++) {
            if (players[i].team == teamName) {
                count++;
            }
        }
        return count;
    }
    
    double getTeamAverageRanking(string teamName) {
        int total = 0, count = 0;
        for (int i = 0; i < playerCount; i++) {
            if (players[i].team == teamName) {
                total += players[i].ranking;
                count++;
            }
        }
        return count > 0 ? (double)total / count : 0;
    }
    
    void updateTeamStandings() {
        // Reset team records
        for (int i = 0; i < teamCount; i++) {
            teams[i].wins = 0;
            teams[i].losses = 0;
        }
        
        // Update from match results
        for (int i = 0; i < matchCount; i++) {
            if (!matches[i].winner.empty()) {
                int winnerIdx = findTeamIndex(matches[i].winner);
                int loserIdx = findTeamIndex(matches[i].loser);
                
                if (winnerIdx != -1) teams[winnerIdx].wins++;
                if (loserIdx != -1) teams[loserIdx].losses++;
            }
        }
        
        sortTeamsByPerformance();
    }
    
    void sortTeamsByPerformance() {
        // Sort teams by wins (desc), then by average ranking (asc - lower is better)
        for (int i = 0; i < teamCount - 1; i++) {
            for (int j = 0; j < teamCount - i - 1; j++) {
                double avg1 = getTeamAverageRanking(teams[j].teamName);
                double avg2 = getTeamAverageRanking(teams[j + 1].teamName);
                
                if (teams[j].wins < teams[j + 1].wins || 
                    (teams[j].wins == teams[j + 1].wins && avg1 > avg2)) { // Changed from avg1 < avg2
                    Team temp = teams[j];
                    teams[j] = teams[j + 1];
                    teams[j + 1] = temp;
                }
            }
        }
    }
    
    void determineCurrentStage() {
        // Check what stage we're currently in based on completed matches
        if (isStageCompleted("Knockout Stage - Final")) {
            currentStage = COMPLETED;
        } else if (hasStageMatches("Knockout Stage - Final") || isStageCompleted("Knockout Stage - 3rd Place")) {
            currentStage = KNOCKOUT_STAGE_FINALS;
        } else if (hasStageMatches("Knockout Stage - 3rd Place") || isStageCompleted("Group Stage")) {
            currentStage = KNOCKOUT_STAGE_3RDPLACE;
        } else if (hasStageMatches("Group Stage") || isStageCompleted("Qualifiers")) {
            currentStage = GROUP_STAGE;
        } else {
            currentStage = QUALIFIERS;
        }
    }
    
    void scheduleMatches() {
        cout << "\n" << string(75, '+') << endl;
        cout << "SCHEDULE MATCHES" << endl;
        cout << string(75, '+') << endl;
        
        if (currentStage == COMPLETED) {
            cout << "Tournament has been completed!" << endl;
            return;
        }
        
        switch (currentStage) {
            case QUALIFIERS:
                scheduleQualifierMatches();
                break;
            case GROUP_STAGE:
                scheduleGroupMatches();
                break;
            case KNOCKOUT_STAGE_3RDPLACE:
                schedule3rdPlaceMatch();
                break;
            case KNOCKOUT_STAGE_FINALS:
                scheduleFinalMatch();
                break;
            default:
                cout << "Invalid tournament stage!" << endl;
                break;
        }

        waitForEnter();
    }
    
    void scheduleQualifierMatches() {
        if (teamCount != 8) {
            cout << "Need exactly 8 teams for tournament. Current teams: " << teamCount << endl;
            return;
        }
        
        if (hasStageMatches("Qualifiers")) {
            cout << "Qualifier matches already scheduled!" << endl;
            return;
        }
        
        cout << "Scheduling Qualifier Matches (8 teams -> 4 advance)" << endl;
        
        // Sort teams by average ranking for fair seeding
        Team sortedTeams[8];
        for (int i = 0; i < teamCount; i++) {
            sortedTeams[i] = teams[i];
        }
        
        // Sort by average ranking (asc - lower ranking is better)
        for (int i = 0; i < teamCount - 1; i++) {
            for (int j = 0; j < teamCount - i - 1; j++) {
                double avg1 = getTeamAverageRanking(sortedTeams[j].teamName);
                double avg2 = getTeamAverageRanking(sortedTeams[j + 1].teamName);
                if (avg1 > avg2) { // Changed from avg1 < avg2
                    Team temp = sortedTeams[j];
                    sortedTeams[j] = sortedTeams[j + 1];
                    sortedTeams[j + 1] = temp;
                }
            }
        }
        
        // Create 4 qualifier matches with fair pairings (similar rankings)
        // Instead of 1vs8, 2vs7, 3vs6, 4vs5, do: 1vs2, 3vs4, 5vs6, 7vs8
        int pairings[4][2] = {{0,1}, {2,3}, {4,5}, {6,7}};
        
        for (int i = 0; i < 4; i++) {
            createMatch(sortedTeams[pairings[i][0]].teamName, 
                    sortedTeams[pairings[i][1]].teamName, "Qualifiers");
            cout << "Match " << (i + 1) << ": " << sortedTeams[pairings[i][0]].teamName 
                << " vs " << sortedTeams[pairings[i][1]].teamName << endl;
        }
        cout << string(75, '~') << endl;
        cout << "4 Qualifier matches scheduled successfully!" << endl;
        
        // Display queue after adding matches
        matchQueue.display();
    }
    
    void scheduleGroupMatches() {
        if (!isStageCompleted("Qualifiers")) {
            cout << "Cannot schedule Group Stage. Complete Qualifiers first!" << endl;
            return;
        }
        
        if (hasStageMatches("Group Stage")) {
            cout << "Group Stage matches already scheduled!" << endl;
            return;
        }
        
        cout << "Scheduling Group Stage Matches (4 teams -> 2 advance)" << endl;
        
        // Get 4 qualified teams
        string qualifiedTeams[4];
        getQualifiedTeams(qualifiedTeams, "Qualifiers", 4);
        
        // Schedule group matches (Group A: teams 0,1; Group B: teams 2,3)
        createMatch(qualifiedTeams[0], qualifiedTeams[1], "Group Stage");
        createMatch(qualifiedTeams[2], qualifiedTeams[3], "Group Stage");
        
        cout << "Group A: " << qualifiedTeams[0] << " vs " << qualifiedTeams[1] << endl;
        cout << "Group B: " << qualifiedTeams[2] << " vs " << qualifiedTeams[3] << endl;
         cout << string(75, '~') << endl;
        cout << "Group Stage matches scheduled successfully!" << endl;

        // Display queue after adding matches
        matchQueue.display();
    }
    
    void schedule3rdPlaceMatch() {
        if (!isStageCompleted("Group Stage")) {
            cout << "Cannot schedule 3rd Place match. Complete Group Stage first!" << endl;
            return;
        }
        
        if (hasStageMatches("Knockout Stage - 3rd Place")) {
            cout << "3rd Place match already scheduled!" << endl;
            return;
        }
        
        cout << "Scheduling 3rd Place Match" << endl;
        
        // Get group stage losers
        string groupLosers[2];
        getGroupLosers(groupLosers);
        
        // Schedule 3rd place match
        createMatch(groupLosers[0], groupLosers[1], "Knockout Stage - 3rd Place");
        cout << "3rd Place Match: " << groupLosers[0] << " vs " << groupLosers[1] << endl;
        
        cout << string(75, '~') << endl;
        cout << "\n3rd Place match scheduled!" << endl;
        // Display queue after adding match
        matchQueue.display();
    }
    
    void scheduleFinalMatch() {
        if (!isStageCompleted("Knockout Stage - 3rd Place")) {
            cout << "Cannot schedule Final. Complete 3rd Place match first!" << endl;
            return;
        }
        
        if (hasStageMatches("Knockout Stage - Final")) {
            cout << "Final match already scheduled!" << endl;
            return;
        }
        
        cout << "Scheduling Final Match" << endl;
        
        // Get group stage winners
        string groupWinners[2];
        getGroupWinners(groupWinners);
        
        // Schedule final
        createMatch(groupWinners[0], groupWinners[1], "Knockout Stage - Final");
        cout << "Final: " << groupWinners[0] << " vs " << groupWinners[1] << endl;
        
        cout << string(75, '~') << endl;
        cout << "Final match scheduled!" << endl;

        // Display queue after adding match
        matchQueue.display();
    }

    // Add this helper function in the private section of your MatchScheduler class:
    string generateRandomDuration() {
        // Generate random minutes between 35-60 minutes
        int minutes = 35 + (rand() % 26); // 35 to 60 minutes
        
        // Generate random seconds between 0-59
        int seconds = rand() % 60;
        
        // Format as MM:SS
        string minStr = (minutes < 10) ? "0" + to_string(minutes) : to_string(minutes);
        string secStr = (seconds < 10) ? "0" + to_string(seconds) : to_string(seconds);
        
        return minStr + ":" + secStr;
    }
    
    void createMatch(string team1, string team2, string stage) {
        MatchResult newMatch;
        newMatch.matchID = Utils::generateMatchID(matchCount);
        newMatch.team1ID = team1;
        newMatch.team2ID = team2;
        newMatch.matchDate = Utils::getCurrentDate();
        newMatch.stage = stage;
        newMatch.duration = generateRandomDuration();
        
        matches[matchCount] = newMatch;
        matchCount++;
        
        // Add detailed match info to queue
        string matchInfo = newMatch.matchID + "," + team1 + "," + team2;
        matchQueue.addQueue(matchInfo);
    }
    
    void processMatches() {
        cout << "\n" << string(75, '+') << endl;
        cout << "PROCESS MATCH RESULTS" << endl;
        cout << string(75, '+') << endl;
        
        // Get pending matches for current stage
        int pendingMatches[10];
        int pendingCount = getPendingMatches(pendingMatches);
        
        if (pendingCount == 0) {
            cout << "No pending matches for current stage!" << endl;
            checkStageProgression();
            return;
        }
        
        displayPendingMatches(pendingMatches, pendingCount);
        
        cout << "Select a match to simulate (1-" << pendingCount << "): ";
        int choice;
        cin >> choice;
        
        if (cin.fail() || choice < 1 || choice > pendingCount) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid selection!" << endl;
            return;
        }
        
        simulateMatch(pendingMatches[choice - 1]);
        checkStageProgression();
        waitForEnter();
    }
    
    int getPendingMatches(int pendingMatches[]) {
        int count = 0;
        string currentStageStr = getStageString(currentStage);
        
        for (int i = 0; i < matchCount; i++) {
            if (matches[i].stage == currentStageStr && matches[i].winner.empty()) {
                pendingMatches[count] = i;
                count++;
            }
        }
        return count;
    }
    
    void displayPendingMatches(int pendingMatches[], int count) {
        cout << "Pending matches for " << getStageString(currentStage) << ":" << endl;
        
        for (int i = 0; i < count; i++) {
            int idx = pendingMatches[i];
            cout << (i + 1) << ". " << matches[idx].matchID << " - ";
            cout << matches[idx].team1ID << " vs " << matches[idx].team2ID << endl;
        }
        cout << string(75, '~') << endl;
    }
    
    void simulateMatch(int matchIndex) {
        MatchResult& match = matches[matchIndex];
        
        // Simulate first-to-5 scoring with tie handling
        double team1Avg = getTeamAverageRanking(match.team1ID);
        double team2Avg = getTeamAverageRanking(match.team2ID);
        
        int score1, score2;
        simulateFirstTo5Score(team1Avg, team2Avg, score1, score2);
        
        match.team1Score = score1;
        match.team2Score = score2;
        
        if (score1 > score2) {
            match.winner = match.team1ID;
            match.loser = match.team2ID;
        } else {
            match.winner = match.team2ID;
            match.loser = match.team1ID;
        }
        
        // Update team standings
        updateTeamStandings();
        
        // Add detailed result to match history stack
        string resultInfo = match.matchID + "," + 
                   match.team1ID + "," + 
                   match.team2ID + "," + 
                   match.winner + "," + 
                   match.loser + "," + 
                   match.matchDate + "," + 
                   match.stage + "," + 
                   to_string(match.team1Score) + "," + 
                   to_string(match.team2Score) + "," + 
                   match.duration;
        matchHistory.push(resultInfo);
        matchHistory.display();
        
        // Remove from queue
        if (!matchQueue.isEmpty()) {
            matchQueue.deleteQueue();
            matchQueue.display();
        }
        
        cout << endl << string(40, '-') << endl;
        cout << "       MATCH RESULT for " << match.matchID << endl;
        cout << string(40, '-') << endl;
        cout << match.team1ID << "     " << score1 << "   -   " << score2 << "     " << match.team2ID << endl;
        cout << string(40, '-') << endl;
        cout << "Winner: " << match.winner << endl;
    }
    
    void displayPlayerStandings() {
        cout << "\nPLAYER STANDINGS BY TEAM" << endl;
        cout << string(75, '~') << endl;
        
        // Update team standings first
        updateTeamStandings();
    
        // Update individual player records and track if any changes were made
        bool playersUpdated = updatePlayerRecords();
        
        cout << "Pos | Team Name                           | Player | W-L  | Win% | Avg Rank" << endl;
        cout << string(75, '-') << endl;
        
        for (int i = 0; i < teamCount; i++) {
            int totalGames = teams[i].wins + teams[i].losses;
            double winPercentage = totalGames > 0 ? (double)teams[i].wins / totalGames * 100 : 0;
            double avgRanking = getTeamAverageRanking(teams[i].teamName);
            
            // Display team header
            cout << (i + 1) << string(3 - to_string(i + 1).length(), ' ') << " | ";
            cout << teams[i].teamName << string(35 - teams[i].teamName.length(), ' ') << " | ";
            cout << teams[i].playerCount << string(6 - to_string(teams[i].playerCount).length(), ' ') << " | ";
            cout << teams[i].wins << "-" << teams[i].losses << string(4 - to_string(teams[i].wins).length() - 1 - to_string(teams[i].losses).length(), ' ') << " | ";
            cout << (int) winPercentage << "%" << string(4 - to_string((int)winPercentage).length() - 1, ' ') << " | ";
            cout << (int) avgRanking << endl;
            
            // Display players for this team
            for (int j = 0; j < playerCount; j++) {
                if (players[j].team == teams[i].teamName) {
                    string playerInfo = players[j].playerID + " - " + players[j].name + " (R: " + to_string(players[j].ranking) + ")";
                    cout << "    |   " << playerInfo << string(32 - playerInfo.length() > 0 ? 32 - playerInfo.length() : 1, ' ') << "  | ";
                    cout << string(6, ' ') << " | ";
                    cout << string(4, ' ') << " | ";
                    cout << string(4, ' ') << " | ";
                    cout << string(8, ' ') << endl;
                }
            }
        }
        cout << string(75, '-') << endl;
            // Display update status
        if (playersUpdated) {
            cout << "Player records updated!" << endl;
        } else {
            cout << "Player records are up-to-date." << endl;
        }
        waitForEnter();
    }

    bool updatePlayerRecords() {
        bool recordsUpdated = false;
        
        // Calculate expected wins/losses for each player based on matches
        for (int i = 0; i < playerCount; i++) {
            int expectedWins = 0;
            int expectedLosses = 0;
            
            // Count wins and losses from completed matches
            for (int j = 0; j < matchCount; j++) {
                if (!matches[j].winner.empty() && !matches[j].loser.empty()) {
                    if (players[i].team == matches[j].winner) {
                        expectedWins++;
                    } else if (players[i].team == matches[j].loser) {
                        expectedLosses++;
                    }
                }
            }
            
            // Check if player's record needs updating
            if (players[i].wins != expectedWins || players[i].losses != expectedLosses) {
                players[i].wins = expectedWins;
                players[i].losses = expectedLosses;
                recordsUpdated = true;
            }
        }
        FileManager::savePlayers(players, playerCount);
        return recordsUpdated;
    }

void displayTournamentBracket() {
    cout << "\nTOURNAMENT BRACKET" << endl;
    cout << string(75, '~') << endl;
    
    // Get match results organized by stage
    string qualifierResults[4][5]; // [match][team1, team2, score1, score2, winner]
    string groupResults[2][5];
    string knockoutResults[2][5]; // 0=3rd place, 1=final
    
    // Initialize arrays
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            qualifierResults[i][j] = "";
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            groupResults[i][j] = "";
            knockoutResults[i][j] = "";
        }
    }
    
    // Fill qualifier results
    int qIndex = 0;
    for (int i = 0; i < matchCount && qIndex < 4; i++) {
        if (matches[i].stage == "Qualifiers") {
            qualifierResults[qIndex][0] = matches[i].team1ID;
            qualifierResults[qIndex][1] = matches[i].team2ID;
            if (!matches[i].winner.empty()) {
                qualifierResults[qIndex][2] = to_string(matches[i].team1Score);
                qualifierResults[qIndex][3] = to_string(matches[i].team2Score);
                qualifierResults[qIndex][4] = matches[i].winner;
            }
            qIndex++;
        }
    }
    
    // Fill group stage results
    int gIndex = 0;
    for (int i = 0; i < matchCount && gIndex < 2; i++) {
        if (matches[i].stage == "Group Stage") {
            groupResults[gIndex][0] = matches[i].team1ID;
            groupResults[gIndex][1] = matches[i].team2ID;
            if (!matches[i].winner.empty()) {
                groupResults[gIndex][2] = to_string(matches[i].team1Score);
                groupResults[gIndex][3] = to_string(matches[i].team2Score);
                groupResults[gIndex][4] = matches[i].winner;
            }
            gIndex++;
        }
    }
    
    // Fill knockout results
    for (int i = 0; i < matchCount; i++) {
        if (matches[i].stage == "Knockout Stage - 3rd Place") {
            knockoutResults[0][0] = matches[i].team1ID;
            knockoutResults[0][1] = matches[i].team2ID;
            if (!matches[i].winner.empty()) {
                knockoutResults[0][2] = to_string(matches[i].team1Score);
                knockoutResults[0][3] = to_string(matches[i].team2Score);
                knockoutResults[0][4] = matches[i].winner;
            }
        }
        if (matches[i].stage == "Knockout Stage - Final") {
            knockoutResults[1][0] = matches[i].team1ID;
            knockoutResults[1][1] = matches[i].team2ID;
            if (!matches[i].winner.empty()) {
                knockoutResults[1][2] = to_string(matches[i].team1Score);
                knockoutResults[1][3] = to_string(matches[i].team2Score);
                knockoutResults[1][4] = matches[i].winner;
            }
        }
    }
    
    // Display the bracket
    displayCleanBracket(qualifierResults, groupResults, knockoutResults);
    
    // Display final results if tournament is completed
    if (currentStage == COMPLETED) {
        cout << string(40, '-') << endl;
        cout << "FINAL RESULTS" << endl;
        cout << string(40, '-') << endl;
        
        string champion = "", second = "", third = "";
        for (int i = 0; i < matchCount; i++) {
            if (matches[i].stage == "Knockout Stage - Final" && !matches[i].winner.empty()) {
                champion = matches[i].winner;
                second = matches[i].loser;
            }
            if (matches[i].stage == "Knockout Stage - 3rd Place" && !matches[i].winner.empty()) {
                third = matches[i].winner;
            }
        }
        
        if (!champion.empty()) {
            cout << "1ST PLACE: " << champion << endl;
            cout << "2ND PLACE: " << second << endl;
            cout << "3RD PLACE: " << third << endl;
        }
        cout << string(40, '-') << endl;
    }

    cout << string(75, '~') << endl;
    waitForEnter();
}

void displayCleanBracket(string qualifierResults[4][5], string groupResults[2][5], string knockoutResults[2][5]) {
    cout << "QUALIFIERS (8 -> 4)      GROUP STAGE (4 -> 2)        KNOCKOUT STAGE" << endl;
    cout << string(75, '=') << endl;
        
    // Qualifier -> Group A
    cout << string(18, ' ') << "__" << endl;
    cout << formatMatchLine(qualifierResults[0][0], qualifierResults[0][2], 18) << "  |" << endl;
    cout << formatMatchLine(qualifierResults[0][1], qualifierResults[0][3], 18) << "  |" << "    GROUP A:" << string(10, ' ') << "__" << endl;
    cout << string(19, ' ') << " |__  " << formatMatchLine(groupResults[0][0], groupResults[0][2], 18) << "  |__" << endl;
    cout << string(19, ' ') << " |    " << formatMatchLine(groupResults[0][1], groupResults[0][3], 18) << "__|" << string(2, ' ') << "|" << endl;
    cout << formatMatchLine(qualifierResults[1][0], qualifierResults[1][2], 18) << "  |" << string(27, ' ') << "|" << endl;
    cout << formatMatchLine(qualifierResults[1][1], qualifierResults[1][3], 18) << "__|" << string(27, ' ') << "|" << "    FINALS:" << endl;
 
    // Finals
    cout << string(48, ' ') << "|__  " << formatMatchLine(knockoutResults[1][0], knockoutResults[1][2], 18) << endl;
    cout << string(18, ' ') << "__  " << string(26, ' ') << "|    " << formatMatchLine(knockoutResults[1][1], knockoutResults[1][3], 18) << endl;

    // Qualifier -> Group B
    cout << formatMatchLine(qualifierResults[2][0], qualifierResults[2][2], 18) << "  |" << string(27, ' ') << "|" << endl;
    cout << formatMatchLine(qualifierResults[2][1], qualifierResults[2][3], 18) << "  |" << "    GROUP B:" << string(10, ' ') << "__" << string(3, ' ') << "|" << endl;
    cout << string(19, ' ') << " |__  " << formatMatchLine(groupResults[1][0], groupResults[1][2], 18) << "  |__|" << "    3RD-PLACE PLAYOFF:" << endl;
    cout << string(19, ' ') << " |    " << formatMatchLine(groupResults[1][1], groupResults[1][3], 18) << "__|" << string(7, ' ') << formatMatchLine(knockoutResults[0][0], knockoutResults[0][2], 18) << endl;
    cout << formatMatchLine(qualifierResults[3][0], qualifierResults[3][2], 18) << "  |" << string(32, ' ') << formatMatchLine(knockoutResults[0][1], knockoutResults[0][3], 18) << endl;
    cout << formatMatchLine(qualifierResults[3][1], qualifierResults[3][3], 18) << "__|" << endl;
}

string formatMatchLine(string teamName, string score, int totalWidth) {
    string result = teamName;

    // Handle empty team name
    if (result.empty()) {
        teamName = "TBD         ";
    }
    
    if (!score.empty()) {
        string scoreStr = "[" + score + "]";
        int availableWidth = totalWidth - scoreStr.length();

            // Pad to exact width
        while (result.length() < totalWidth) {
         result = teamName + string(availableWidth - teamName.length(), ' ') + scoreStr;
    }
    }
    else {
        // No score - just pad team name to full width
        result = teamName + string(totalWidth - teamName.length(), ' ');
    }
    
    return result;
}
    
    // Helper functions
    bool hasStageMatches(string stage) {
        for (int i = 0; i < matchCount; i++) {
            if (matches[i].stage == stage) {
                return true;
            }
        }
        return false;
    }
    
    bool isStageCompleted(string stage) {
        int totalMatches = 0, completedMatches = 0;
        
        for (int i = 0; i < matchCount; i++) {
            if (matches[i].stage == stage) {
                totalMatches++;
                if (!matches[i].winner.empty()) {
                    completedMatches++;
                }
            }
        }
        
        return totalMatches > 0 && totalMatches == completedMatches;
    }
    
    void getQualifiedTeams(string qualified[], string fromStage, int maxTeams) {
        int count = 0;
        for (int i = 0; i < matchCount && count < maxTeams; i++) {
            if (matches[i].stage == fromStage && !matches[i].winner.empty()) {
                qualified[count] = matches[i].winner;
                count++;
            }
        }
    }
    
    void getGroupWinners(string winners[]) {
        int count = 0;
        for (int i = 0; i < matchCount && count < 2; i++) {
            if (matches[i].stage == "Group Stage" && !matches[i].winner.empty()) {
                winners[count] = matches[i].winner;
                count++;
            }
        }
    }
    
    void getGroupLosers(string losers[]) {
        int count = 0;
        for (int i = 0; i < matchCount && count < 2; i++) {
            if (matches[i].stage == "Group Stage" && !matches[i].loser.empty()) {
                losers[count] = matches[i].loser;
                count++;
            }
        }
    }
    
    int findTeamIndex(string teamName) {
        for (int i = 0; i < teamCount; i++) {
            if (teams[i].teamName == teamName) {
                return i;
            }
        }
        return -1;
    }
    
    void simulateFirstTo5Score(double team1Avg, double team2Avg, int& score1, int& score2) {
        score1 = 0;
        score2 = 0;
        
        // Calculate win probability based on ranking difference (lower ranking is better)
        double rankDiff = team2Avg - team1Avg; // Reversed: team2 - team1
        double team1WinProb = 0.5 + (rankDiff / 1000.0) * 0.3; // Lower team1Avg gives higher probability
        
        // Clamp probability between 0.2 and 0.8 for realistic matches
        if (team1WinProb > 0.8) team1WinProb = 0.8;
        if (team1WinProb < 0.2) team1WinProb = 0.2;
        
        // Play until one team reaches 5 points
        while (score1 < 5 && score2 < 5) {
            if ((double)rand() / RAND_MAX < team1WinProb) {
                score1++;
            } else {
                score2++;
            }
        }
    }
    
    void checkStageProgression() {
        string currentStageStr = getStageString(currentStage);
        
        if (isStageCompleted(currentStageStr)) {           
            // Progress to next stage
            switch (currentStage) {
                case QUALIFIERS:
                    currentStage = GROUP_STAGE;
                    cout << "Stage Completed! Advancing to Group Stage..." << endl;
                    break;
                case GROUP_STAGE:
                    currentStage = KNOCKOUT_STAGE_3RDPLACE;
                    cout << "Stage Completed! Advancing to Knockout Stage - 3rd Place Match..." << endl;
                    break;
                case KNOCKOUT_STAGE_3RDPLACE:
                    currentStage = KNOCKOUT_STAGE_FINALS;
                    cout << "Stage Completed! Advancing to Knockout Stage - Finals..." << endl;
                    break;
                case KNOCKOUT_STAGE_FINALS:
                    currentStage = COMPLETED;
                    cout << "Tournament Completed!" << endl;
                    break;
                default:
                    break;
            }
        }
    }
};