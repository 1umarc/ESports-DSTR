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
    string team; 
    int ranking;
    string registrationDate;
    string registrationType; // Replaces isEarlyBird and isWildcard
    string status;           // Replaces isCheckedIn
    int wins;
    int losses;

    Player() {
        playerID = "";
        name = "";
        email = "";
        university = "";
        team = "";  
        ranking = 0;
        registrationDate = "";
        registrationType = "";  // e.g., "Early-Bird", "Wildcard", "Regular"
        status = "Registered";  // e.g., "Registered", "Checked-In", "Withdrawn"
        wins = 0;
        losses = 0;
    }

    Player(string id, string n, string e, string uni, string t,int rank, string regDate, string regType = "Regular", string stat = "Registered") {
        playerID = id;
        name = n;
        email = e;
        university = uni;
        ranking = rank;
        team = t;  
        registrationDate = regDate;
        registrationType = regType;
        status = stat;
        wins = 0;
        losses = 0;
    }
};

// Team structure for tournament progression
struct Team {
    string teamName;
    int playerCount;
    int wins;
    int losses;
    
    Team() {
        teamName = "";
        playerCount = 0;
        wins = 0;
        losses = 0;
    }
    
    Team(string name, int count) {
        teamName = name;
        playerCount = count;
        wins = 0;
        losses = 0;
    }
};

// Match structure
struct MatchResult {
    string matchID;
    string team1ID;
    string team2ID;
    string winner;
    string loser;
    string matchDate;
    string stage; // qualifier, group, knockout
    int team1Score;
    int team2Score;
    string duration;
    
    MatchResult() {
        matchID = "";
        team1ID = "";
        team2ID = "";
        winner = "";
        loser = "";
        matchDate = "";
        stage = "";
        team1Score = 0;
        team2Score = 0;
        duration = "";
    }
    
    MatchResult(string id, string t1, string t2, string date, string stg) {
        matchID = id;
        team1ID = t1;
        team2ID = t2;
        winner = "";
        loser = "";
        matchDate = date;
        stage = stg;
        team1Score = 0;
        team2Score = 0;
        duration = "";
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

