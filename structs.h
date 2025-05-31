#pragma once
#include <string>
#include <iostream>
using namespace std;

// Player structure
struct Player {
    string playerID;
    string name;
    string email;
    string university;
    int ranking;
    string registrationDate;
    bool isCheckedIn;
    bool isEarlyBird;
    bool isWildcard;
    int wins;
    int losses;
    
    Player() {
        playerID = "";
        name = "";
        email = "";
        university = "";
        ranking = 0;
        registrationDate = "";
        isCheckedIn = false;
        isEarlyBird = false;
        isWildcard = false;
        wins = 0;
        losses = 0;
    }
    
    Player(string id, string n, string e, string uni, int rank, string regDate, bool earlyBird = false, bool wildcard = false) {
        playerID = id;
        name = n;
        email = e;
        university = uni;
        ranking = rank;
        registrationDate = regDate;
        isCheckedIn = false;
        isEarlyBird = earlyBird;
        isWildcard = wildcard;
        wins = 0;
        losses = 0;
    }
};

// Match structure
struct Match {
    string matchID;
    string player1ID;
    string player2ID;
    string winner;
    string loser;
    string matchDate;
    string stage; // qualifier, group, knockout
    int player1Score;
    int player2Score;
    string duration;
    
    Match() {
        matchID = "";
        player1ID = "";
        player2ID = "";
        winner = "";
        loser = "";
        matchDate = "";
        stage = "";
        player1Score = 0;
        player2Score = 0;
        duration = "";
    }
    
    Match(string id, string p1, string p2, string date, string stg) {
        matchID = id;
        player1ID = p1;
        player2ID = p2;
        winner = "";
        loser = "";
        matchDate = date;
        stage = stg;
        player1Score = 0;
        player2Score = 0;
        duration = "";
    }
};

// Tournament Record structure
struct TournamentRecord {
    string recordID;
    string tournamentName;
    string date;
    string winner;
    string runnerUp;
    int totalParticipants;
    string status; // ongoing, completed
    
    TournamentRecord() {
        recordID = "";
        tournamentName = "";
        date = "";
        winner = "";
        runnerUp = "";
        totalParticipants = 0;
        status = "";
    }
    
    TournamentRecord(string id, string name, string d, int participants) {
        recordID = id;
        tournamentName = name;
        date = d;
        winner = "";
        runnerUp = "";
        totalParticipants = participants;
        status = "ongoing";
    }
};

// Audience structure  
struct Audience {
    string audienceID;
    string name;
    string email;
    string ticketType; // VIP, Streamer, General
    int priority; // 1=VIP, 2=Streamer, 3=General
    string seatNumber;
    bool isSeated;
    
    Audience() {
        audienceID = "";
        name = "";
        email = "";
        ticketType = "";
        priority = 3;
        seatNumber = "";
        isSeated = false;
    }
    
    Audience(string id, string n, string e, string type, int prio) {
        audienceID = id;
        name = n;
        email = e;
        ticketType = type;
        priority = prio;
        seatNumber = "";
        isSeated = false;
    }
};

// Admin structure
struct Admin {
    string adminID;
    string username;
    string password;
    string name;
    
    Admin() {
        adminID = "";
        username = "";
        password = "";
        name = "";
    }
    
    Admin(string id, string user, string pass, string n) {
        adminID = id;
        username = user;
        password = pass;
        name = n;
    }
};

// Match Result structure for logging
struct MatchResult {
    string matchID;
    string player1;
    string player2;
    string winner;
    string stage;
    string timestamp;
    int score1;
    int score2;
    
    MatchResult() {
        matchID = "";
        player1 = "";
        player2 = "";
        winner = "";
        stage = "";
        timestamp = "";
        score1 = 0;
        score2 = 0;
    }
    
    MatchResult(string id, string p1, string p2, string win, string stg, string time, int s1, int s2) {
        matchID = id;
        player1 = p1;
        player2 = p2;
        winner = win;
        stage = stg;
        timestamp = time;
        score1 = s1;
        score2 = s2;
    }
};
