#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include "structs.h"

using namespace std;

class FileManager {
public:
    // Read players from CSV
    static void loadPlayers(Player players[], int& count) {
        ifstream file("Player.csv");
        string line;
        count = 0;
        
        if (!file.is_open()) {
            cout << "Warning: Player.csv not found. Using empty player database." << endl;
            return;
        }
        
        // Skip header
        getline(file, line);
        
        while (getline(file, line) && count < 100) {
            stringstream ss(line);
            string field;
            
            getline(ss, players[count].playerID, ',');
            getline(ss, players[count].name, ',');
            getline(ss, players[count].email, ',');
            getline(ss, players[count].university, ',');
            getline(ss, players[count].team, ',');
            getline(ss, field, ','); players[count].ranking = stoi(field);
            getline(ss, players[count].registrationDate, ',');
            getline(ss, players[count].registrationType, ',');
            getline(ss, players[count].status, ',');
            getline(ss, field, ','); players[count].wins = stoi(field);
            getline(ss, field, ','); players[count].losses = stoi(field);
            
            count++;
        }
        file.close();
        cout << "Loaded " << count << " players from database." << endl;
    }
    
    // Save players to CSV
    static void savePlayers(Player players[], int count) {
        ofstream file("Player.csv");
        if (!file.is_open()) {
            cout << "Error: Cannot save to Player.csv" << endl;
            return;
        }
        
        file << "PlayerID,Name,Email,University,Team,Ranking,RegistrationDate,RegistrationType,Status,Wins,Losses\n";
        for (int i = 0; i < count; i++) {
            file << players[i].playerID << ","
                 << players[i].name << ","
                 << players[i].email << ","
                 << players[i].university << ","
                 << players[i].team << ","
                 << players[i].ranking << ","
                 << players[i].registrationDate << ","
                << players[i].registrationType << ","
                << players[i].status << ","
                 << players[i].wins << ","
                 << players[i].losses << "\n";
        }
        file.close();
    }
    
    // Read audience from CSV
    static void loadAudience(Audience audience[], int& count) {
        ifstream file("Audience_Ticket.csv");
        string line;
        count = 0;
        
        if (!file.is_open()) {
            cout << "Warning: Audience_Ticket.csv not found. Using empty audience database." << endl;
            return;
        }
        
        // Skip header
        getline(file, line);
        
        while (getline(file, line) && count < 100) {
            stringstream ss(line);
            string field;
            
            getline(ss, audience[count].audienceID, ',');
            getline(ss, audience[count].name, ',');
            getline(ss, audience[count].email, ',');
            getline(ss, audience[count].ticketType, ',');
            getline(ss, field, ','); audience[count].priority = stoi(field);
            getline(ss, audience[count].seatNumber, ',');
            getline(ss, field, ','); audience[count].isSeated = (field == "1");
            
            count++;
        }
        file.close();
        cout << "Loaded " << count << " audience members from database." << endl;
    }
    
    // Save audience to CSV
    static void saveAudience(Audience audience[], int count) {
        ofstream file("Audience_Ticket.csv");
        if (!file.is_open()) {
            cout << "Error: Cannot save to Audience_Ticket.csv" << endl;
            return;
        }
        
        file << "AudienceID,Name,Email,TicketType,Priority,SeatNumber,IsSeated\n";
        for (int i = 0; i < count; i++) {
            file << audience[i].audienceID << ","
                 << audience[i].name << ","
                 << audience[i].email << ","
                 << audience[i].ticketType << ","
                 << audience[i].priority << ","
                 << audience[i].seatNumber << ","
                 << (audience[i].isSeated ? 1 : 0) << "\n";
        }
        file.close();
    }
    
    // Read tournament records from CSV
    static void loadMatchResults(MatchResult results[], int& count) 
    {
        ifstream file("Match_Results.csv");
        string line;
        count = 0;
        
        if (!file.is_open()) 
        {
            cout << "Warning: Match_Results.csv not found." << endl;
            return;
        }
        
        // Skip header
        getline(file, line);
        
        while (getline(file, line) && count < 50) 
        {
            stringstream ss(line);
            string field; 
        
            getline(ss, results[count].matchID, ',');
            getline(ss, results[count].team1ID, ',');
            getline(ss, results[count].team2ID, ',');
            getline(ss, results[count].winner, ',');
            getline(ss, results[count].loser, ',');
            getline(ss, results[count].matchDate, ',');
            getline(ss, results[count].stage, ',');
            getline(ss, field, ','); results[count].team1Score = stoi(field);
            getline(ss, field, ','); results[count].team2Score = stoi(field);
            getline(ss, results[count].duration, ',');
            
            count++;
        }
        file.close();
        cout << "Loaded " << count << " Match result from database." << endl;
    }
    
    // Save tournament records to CSV
    static void saveMatchResults(MatchResult results[], int count) 
    {
        ofstream file("Match_Results.csv");
        if (!file.is_open()) 
        {
            cout << "Error: Cannot save to Match_Results.csv" << endl;
            return;
        }
        
        file << "MatchID,Team1ID,Team2ID,Winner,Loser,MatchDate,Stage,Team1Score,Team2Score,Duration\n";
        
        for (int i = 0; i < count; i++) 
        {
            file << results[i].matchID << ","
                << results[i].team1ID << ","
                << results[i].team2ID << ","
                << results[i].winner << ","
                << results[i].loser << ","
                << results[i].matchDate << ","
                << results[i].stage << ","
                << results[i].team1Score << ","
                << results[i].team2Score << ","
                << results[i].duration << "\n";
        }
        file.close();
        cout << "Saved " << count << " match results to database." << endl;
    }
};

class Utils {
public:
    // Get current timestamp
    static string getCurrentTimestamp() {
        time_t now = time(0);
        char* timeStr = ctime(&now);
        string timestamp(timeStr);
        // Remove newline character
        timestamp.pop_back();
        return timestamp;
    }
    
    // Get current date in YYYY-MM-DD format
    static string getCurrentDate() {
        time_t now = time(0);
        struct tm* timeInfo = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeInfo);
        return string(buffer);
    }
    
    // Generate random match ID
    static string generateMatchID() {
        return "M" + to_string(rand() % 9000 + 1000);
    }
    
    // // Generate random player ID
    // static string generatePlayerID(int count) {
    //     return string("P") + (count < 10 ? "00" : (count < 100 ? "0" : "")) + to_string(count + 1);
    // }
    
    // Generate random audience ID
    static string generateAudienceID(int count) {
        return string("A") + (count < 10 ? "00" : (count < 100 ? "0" : "")) + to_string(count + 1);
    }
    
    // Find player by ID
    static int findPlayerIndex(Player players[], int count, string playerID) {
        for (int i = 0; i < count; i++) {
            if (players[i].playerID == playerID) {
                return i;
            }
        }
        return -1;
    }
    
    // Find audience by ID
    static int findAudienceIndex(Audience audience[], int count, string audienceID) {
        for (int i = 0; i < count; i++) {
            if (audience[i].audienceID == audienceID) {
                return i;
            }
        }
        return -1;
    }
    
    // Validate admin credentials
    static bool validateAdmin(string username, string password) {
        // Hardcoded admin credentials
        return (username == "admin" && password == "admin123");
    }
    
    // Clear screen function
    static void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    // Pause function
    static void pauseScreen() {
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    // Convert string to lowercase
    static string toLowerCase(string str) {
        for (int i = 0; i < str.length(); i++) {
            str[i] = tolower(str[i]);
        }
        return str;
    }

    // Validate email format (basic check)
    static bool isValidEmail(const string& email) {
        // Check if email contains @ and has characters before and after @
        size_t atPos = email.find('@');
        if (atPos == string::npos || atPos == 0 || atPos == email.length() - 1) {
            return false;
        }
        
        // Check if there's a dot after @
        size_t dotPos = email.find('.', atPos);
        if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == email.length() - 1) {
            return false;
        }
        
        return true;
    }

    // Get email with validation and convert to lowercase
    static string getEmailInput() {
        string email;
        while (true) {
            cout << "Enter email: ";
            getline(cin, email);
            
            if (email.empty()) {
                cout << "Email cannot be empty. Please try again.\n";
                continue;
            }
            
            if (!isValidEmail(email)) {
                cout << "Invalid email format. Please enter a valid email (e.g., user@domain.com).\n";
                continue;
            }
            
            return toLowerCase(email);
        }
    }

    // Get ranking with validation
    static int getRankingInput() {
        int ranking;
        while (true) {
            cout << "Enter ranking (1000-2000): ";
            cin >> ranking;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input! Please enter a number.\n";
                continue;
            }
            
            if (ranking < 1000 || ranking > 2000) {
                cout << "Ranking must be between 1000 and 2000. Please try again.\n";
                continue;
            }
            
            return ranking;
        }
    }
    
    // Generate random player ID
    static string generatePlayerID(Player players[], int count) {
        int maxID = 0;
        for (int i = 0; i < count; i++) {
            string idNumStr = players[i].playerID.substr(1); // remove 'P'
            int idNum = stoi(idNumStr);
            if (idNum > maxID) maxID = idNum;
        }

        int newID = maxID + 1;

        string idStr = "P";
        if (newID < 10) idStr += "00";
        else if (newID < 100) idStr += "0";
        idStr += to_string(newID);

        return idStr;
    }
};
