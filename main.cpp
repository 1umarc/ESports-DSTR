#include <iostream>
#include "lv.h"
#include "ck.h"
#include "dn.h"
#include "xw.h"

using namespace std;

int main() {
    MatchScheduler ms;
    RegistrationManager rm;
    SpectatorManager sm;
    ResultLogger rl;

    int choice;

    do {
        cout << "\n=== APUEC Championship Management System ===\n";
        cout << "1. Match Scheduling & Player Progression\n";
        cout << "2. Tournament Registration & Player Queueing\n";
        cout << "3. Live Stream & Spectator Queue Management\n";
        cout << "4. Game Result Logging & Performance History\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: ms.runTask(); break;
            case 2: rm.runTask(); break;
            case 3: sm.runTask(); break;
            case 4: rl.runTask(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid option!\n";
        }
    } while (choice != 0);

    return 0;
}
