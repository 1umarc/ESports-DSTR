#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "task1_luven.h"
#include "task2_chunkit.h"
#include "task3_nesya.h"
#include "task4_xiaowen.h"

using namespace std;

MatchScheduler ms;
RegistrationManager rm;
SpectatorManager sm;
ResultLogger rl;

// Global variables for user session
string currentUserID = "";
string currentUserType = "";
string currentUserName = "";

// ----------- Function Prototypes ----------
void displayWelcomeHeader();
void displayMainMenu();
void handleUserChoice(int choice);
bool loginSystem();
void signUpSystem();
void adminMenu();
void playerMenu();
void audienceMenu();
int getValidInput(int min, int max);;
// ------------------------------------------

void displayWelcomeHeader() {
    Utils::clearScreen();
    cout << "\n" << string(60, '=') << endl;
    cout << "    WELCOME TO APUEC CHAMPIONSHIP MANAGEMENT SYSTEM" << endl;
    cout << "     Asia Pacific University Esports Championship" << endl;
    cout << string(60, '=') << endl;
}

void displayMainMenu() {
    cout << "\n" << string(50, '-') << endl;
    cout << "                   MAIN MENU" << endl;
    cout << string(50, '-') << endl;
    cout << "1. Login" << endl;
    cout << "2. Sign Up" << endl;
    cout << "0. Exit System" << endl;
    cout << string(50, '-') << endl;
}

int getValidInput(int min, int max) {
    int choice;
    while (true) {
        cout << "Enter your choice (" << min << "-" << max << "): ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between " << min << " and " << max << "." << endl;
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        
        if (choice >= min && choice <= max) {
            return choice;
        }
        
        cout << "Invalid choice! Please select a number between " << min << " and " << max << "." << endl;
    }
}

void signUpSystem() {
    Utils::clearScreen();
    cout << "\n" << string(50, '=') << endl;
    cout << "    APUEC Championship Management System" << endl;
    cout << "                 SIGN UP" << endl;
    cout << string(50, '=') << endl;
    cout << "1. Register as Player" << endl;
    cout << "2. Register as Audience" << endl;
    cout << "0. Back to Main Menu" << endl;
    cout << string(50, '-') << endl;
    
    int choice = getValidInput(0, 2);
    
    switch (choice) {
        case 1:
            cout << "\nRedirecting to Player Registration..." << endl;
            rm.registerNewPlayer();
            break;
        case 2:
            cout << "\nRedirecting to Audience Registration..." << endl;
            sm.runAudience();
            break;
        case 0:
            return;
        default:
            cout << "Invalid option!" << endl;
            break;
    }
    Utils::pauseScreen();
}

bool loginSystem() {
    Utils::clearScreen();
    
    // Load data from CSV files
    Player players[100];
    Audience audience[1000];
    int playerCount = 0, audienceCount = 0;
    
    // Load players and audience data
    FileManager::loadPlayers(players, playerCount);
    FileManager::loadAudience(audience, audienceCount);
    
    cout << "\n" << string(50, '=') << endl;
    cout << "    APUEC Championship Management System" << endl;
    cout << "                 LOGIN" << endl;
    cout << string(50, '=') << endl;
    cout << "Enter your ID to login (or 'back' to return): ";
    
    string id;
    getline(cin, id);
    
    if (id == "back" || id == "BACK") {
        return false;
    }
    
    // Check if it's admin login
    if (id == "admin") {
        string password;
        cout << "Enter admin password: ";
        getline(cin, password);
        
        if (Utils::validateAdmin(id, password)) {
            currentUserID = "admin";
            currentUserType = "admin";
            currentUserName = "Administrator";
            
            cout << "\n" << string(40, '*') << endl;
            cout << "    ADMIN LOGIN SUCCESSFUL!" << endl;
            cout << "    Welcome, " << currentUserName << "!" << endl;
            cout << string(40, '*') << endl;
            Utils::pauseScreen();
            adminMenu();
            return true;
        } else {
            cout << "\nInvalid admin credentials!" << endl;
            Utils::pauseScreen();
            return false;
        }
    }
    
    // Check if it's a player ID
    int playerIndex = Utils::findPlayerIndex(players, playerCount, id);
    if (playerIndex != -1) {
        currentUserID = id;
        currentUserType = "player";
        currentUserName = players[playerIndex].name;
        
        cout << "\n" << string(40, '*') << endl;
        cout << "    PLAYER LOGIN SUCCESSFUL!" << endl;
        cout << "    Welcome, " << currentUserName << "!" << endl;
        cout << "    Team: " << players[playerIndex].team << endl;
        cout << "    Status: " << players[playerIndex].status << endl;
        cout << "    Ranking: " << players[playerIndex].ranking << endl;
        cout << string(40, '*') << endl;
        Utils::pauseScreen();
        playerMenu();
        return true;
    }
    
    // Check if it's an audience ID
    int audienceIndex = Utils::findAudienceIndex(audience, audienceCount, id);
    if (audienceIndex != -1) {
        currentUserID = id;
        currentUserType = "audience";
        currentUserName = audience[audienceIndex].name;
        
        cout << "\n" << string(40, '*') << endl;
        cout << "    AUDIENCE LOGIN SUCCESSFUL!" << endl;
        cout << "    Welcome, " << currentUserName << "!" << endl;
        cout << "    Ticket Type: " << audience[audienceIndex].ticketType << endl;
        cout << "    Seat Number: " << audience[audienceIndex].seatNumber << endl;
        cout << string(40, '*') << endl;
        Utils::pauseScreen();
        audienceMenu();
        return true;
    }
    
    // If no match found
    cout << "\nID not found! Please check your ID and try again." << endl;
    cout << "Make sure you enter the correct:" << endl;
    cout << "Player ID (P001, P002, etc.)" << endl;
    cout << "Audience ID (A001, A002, etc.)" << endl;
    cout << "Admin ID (admin)" << endl;
    Utils::pauseScreen();
    return false;
}

void adminMenu() {
    int choice;
    string currentPlayerID;
    
    do {
        Utils::clearScreen();
        cout << "\n" << string(50, '=') << endl;
        cout << "           ADMIN CONTROL PANEL" << endl;
        cout << "    Welcome, " << currentUserName << "!" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Match Scheduling & Player Progression" << endl;
        cout << "2. Tournament Registration Management" << endl;
        cout << "3. Spectator Queue Management" << endl;
        cout << "4. Game Result Logging & History" << endl;
        cout << "0. Logout" << endl;
        cout << string(50, '-') << endl;
        
        choice = getValidInput(0, 5);
        
        switch (choice) {
            case 1:
                ms.runAdmin();
                break;
            case 2: {
                rm.adminMenu();
                break;
            }
            case 3: {
                sm.runAdmin();
                break;
            }
            case 4: {
                rl.runAdmin();
                break;
            }
            case 0:
                cout << "\nLogging out..." << endl;
                currentUserID = "";
                currentUserType = "";
                currentUserName = "";
                break;
        }
    } while (choice != 0);
}

void playerMenu() {
    int choice;
    string currentPlayerID = currentUserID; 
   
    do {
        Utils::clearScreen();
        cout << "\n" << string(50, '=') << endl;
        cout << "              PLAYER MENU" << endl;
        cout << "    Welcome, " << currentUserName << "!" << endl;
        cout << "    Player ID: " << currentUserID << endl;
        cout << string(50, '=') << endl;
        cout << "1. Match Scheduling & Player Progression" << endl;
        cout << "2. Tournament Registration & Player Queueing" << endl;
        cout << "3. Game Result Logging & History" << endl;
        cout << "0. Logout" << endl;
        cout << string(50, '-') << endl;

        choice = getValidInput(0, 4);
        
        switch (choice) {
            case 1:
                ms.runPlayer();
                break;
            case 2:
                rm.playerMenu(currentPlayerID);
                break;
            case 3: {
                rl.runPlayer();
                break;
            }
            case 0:
                cout << "\nLogging out..." << endl;
                currentUserID = "";
                currentUserType = "";
                currentUserName = "";
                break;
        }
    } while (choice != 0);
}

void audienceMenu() {
    Utils::clearScreen();
    cout << "\n" << string(50, '=') << endl;
    cout << "             AUDIENCE MENU" << endl;
    cout << "    Welcome, " << currentUserName << "!" << endl;
    cout << "    Audience ID: " << currentUserID << endl;
    cout << string(50, '=') << endl;
    sm.runAudience(); 
}

void handleUserChoice(int choice) {
    switch (choice) {
        case 1:
            loginSystem();
            break;
        case 2:
            signUpSystem();
            break;
        case 0:
            cout << "\n" << string(60, '=') << endl;
            cout << "  Thank you for using APUEC Management System!" << endl;
            cout << "           Tournament session ended." << endl;
            cout << "               See you next time!" << endl;
            cout << string(60, '=') << endl;
            break;
        default:
            cout << "Invalid option! Please try again." << endl;
            break;
    }
}

int main() {
    // Initialize random seed for match simulation
    srand(time(0));

    // Pass the match history stack from MatchScheduler to ResultLogger
    rl.setStack(ms.getStack());
    
    int choice;
    
    do {
        displayWelcomeHeader();
        displayMainMenu();
        choice = getValidInput(0, 3);
        handleUserChoice(choice);
    } while (choice != 0);
    
    return 0;
}