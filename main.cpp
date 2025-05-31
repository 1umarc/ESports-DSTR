#include <iostream>
#include <cstdlib>
#include <ctime>
#include "lvE.h"
#include "ckE.h"
#include "dnE.h"
#include "xwE.h"

using namespace std;

int main() {
    // Initialize random seed for match simulation
    srand(time(0));
    
    // Create instances of all task managers
    MatchScheduler ms;
    RegistrationManager rm;
    SpectatorManager sm;
    ResultLogger rl;

    int choice;
    
    cout << "=== Welcome to APUEC Championship Management System ===" << endl;
    cout << "Initializing tournament management system..." << endl;
    cout << "System ready!" << endl;

    do {
        cout << "\n" << string(50, '=') << endl;
        cout << "    APUEC Championship Management System" << endl;
        cout << string(50, '=') << endl;
        cout << "1. Match Scheduling & Player Progression" << endl;
        cout << "2. Tournament Registration & Player Queueing" << endl;
        cout << "3. Live Stream & Spectator Queue Management" << endl;
        cout << "4. Game Result Logging & Performance History" << endl;
        cout << "0. Exit System" << endl;
        cout << string(50, '-') << endl;
        cout << "Enter your choice (0-4): ";
        cin >> choice;

        // Input validation
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number between 0-4." << endl;
            continue;
        }

        switch (choice) {
            case 1: 
                ms.runTask(); 
                break;
            case 2: 
                rm.runTask(); 
                break;
            case 3: 
                sm.runTask(); 
                break;
            case 4: 
                rl.runTask(); 
                break;
            case 0: 
                cout << "\n" << string(50, '=') << endl;
                cout << "  Thank you for using APUEC Management System!" << endl;
                cout << "           Tournament session ended." << endl;
                cout << string(50, '=') << endl;
                break;
            default: 
                cout << "Invalid option! Please select a number between 0-4." << endl;
                break;
        }
    } while (choice != 0);

    return 0;
}