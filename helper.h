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
            getline(ss, field, ','); players[count].ranking = stoi(field);
            getline(ss, players[count].registrationDate, ',');
            getline(ss, field, ','); players[count].isEarlyBird = (field == "1");
            getline(ss, field, ','); players[count].isWildcard = (field == "1");
            getline(ss, field, ','); players[count].wins = stoi(field);
            getline(ss, field, ','); players[count].losses = stoi(field);
            
            players[count].isCheckedIn = false;
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
        
        file << "PlayerID,Name,Email,University,Ranking,RegistrationDate,IsEarlyBird,IsWildcard,Wins,Losses\n";
        for (int i = 0; i < count; i++) {
            file << players[i].playerID << ","
                 << players[i].name << ","
                 << players[i].email << ","
                 << players[i].university << ","
                 << players[i].ranking << ","
                 << players[i].registrationDate << ","
                 << (players[i].isEarlyBird ? 1 : 0) << ","
                 << (players[i].isWildcard ? 1 : 0) << ","
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
    static void loadTournamentRecords(TournamentRecord records[], int& count) {
        ifstream file("Tournament_Record.csv");
        string line;
        count = 0;
        
        if (!file.is_open()) {
            cout << "Warning: Tournament_Record.csv not found. Using empty tournament database." << endl;
            return;
        }
        
        // Skip header
        getline(file, line);
        
        while (getline(file, line) && count < 50) {
            stringstream ss(line);
            string field;
            
            getline(ss, records[count].recordID, ',');
            getline(ss, records[count].tournamentName, ',');
            getline(ss, records[count].date, ',');
            getline(ss, records[count].winner, ',');
            getline(ss, records[count].runnerUp, ',');
            getline(ss, field, ','); records[count].totalParticipants = stoi(field);
            getline(ss, records[count].status, ',');
            
            count++;
        }
        file.close();
        cout << "Loaded " << count << " tournament records from database." << endl;
    }
    
    // Save tournament records to CSV
    static void saveTournamentRecords(TournamentRecord records[], int count) {
        ofstream file("Tournament_Record.csv");
        if (!file.is_open()) {
            cout << "Error: Cannot save to Tournament_Record.csv" << endl;
            return;
        }
        
        file << "RecordID,TournamentName,Date,Winner,RunnerUp,TotalParticipants,Status\n";
        for (int i = 0; i < count; i++) {
            file << records[i].recordID << ","
                 << records[i].tournamentName << ","
                 << records[i].date << ","
                 << records[i].winner << ","
                 << records[i].runnerUp << ","
                 << records[i].totalParticipants << ","
                 << records[i].status << "\n";
        }
        file.close();
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
    
    // Generate random player ID
    static string generatePlayerID(int count) {
        return string("P") + (count < 10 ? "00" : (count < 100 ? "0" : "")) + to_string(count + 1);
    }
    
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
};
