#include "FleetManager.h"
#include <algorithm>
#include <iostream>
using namespace std;

FleetManager::FleetManager() : balance(100), nextId(101), saveFileName("") 
{
}

FleetManager::~FleetManager() 
{
    fleet.clear();
}
void FleetManager::showFleet()
{
}
void FleetManager::addUnit()
{
}
void FleetManager::deployMission()
{
}
void FleetManager::upgradeCenter() {}
void FleetManager::saveFleet() {}
void FleetManager::loadFleet() {}
void FleetManager::clearScreen() 
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void FleetManager::run() 
{
    bool running = true;
    while (running) 
    {
        clearScreen();
        cout << "========================================\n";
        cout << "       AEGIS TACTICAL COMMAND\n";
        cout << "========================================\n";
        cout << "Balance: " << balance << " coins | Drones: " << fleet.size() << "\n";
        cout << "----------------------------------------\n";
        cout << "1. Show my Fleet\n";
        cout << "2. Add Unit to my Fleet\n";
        cout << "3. Deploy for Mission\n";
        cout << "4. Upgrade Center\n";
        cout << "5. Save Fleet to File\n";
        cout << "6. Load Fleet from File\n";
        cout << "7. Exit\n";
        cout << "----------------------------------------\n<< ";

        string line;
        getline(cin, line);
        int choice = 0;
        try { choice = stoi(line); }
        catch (...) { continue; }

        switch (choice) {
        case 1: showFleet();      break;
        case 2: addUnit();        break;
        case 3: deployMission();  break;
        case 4: upgradeCenter();  break;
        case 5: saveFleet();      break;
        case 6: loadFleet();      break;
        case 7:
            cout << "Exiting without save\n";
            running = false;
            break;
        default:
            cout << "Invalid option.\n";
        }
    }
}