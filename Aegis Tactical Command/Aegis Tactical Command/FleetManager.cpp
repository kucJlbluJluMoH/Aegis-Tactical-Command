#include "FleetManager.h"
#include <algorithm>
#include <iostream>
using namespace std;

FleetManager::FleetManager() : balance(100), nextId(101), saveFileName("") 
{
    srand(static_cast<unsigned>(time(nullptr)));
    fleet.push_back(new CombatDrone(101, 100, 30, 70));
    fleet.push_back(new CombatDrone(102, 85, 30, 70));
    fleet.push_back(new CombatDrone(103, 70, 30, 70));
    fleet.push_back(new CombatDrone(104, 13, 40, 70));
    fleet.push_back(new CombatDrone(105, 50, 50, 70));
    fleet.push_back(new ScoutDrone(106, 90, 3, 1.1));
    fleet.push_back(new ChargerDrone(107, 70, 100));
    nextId = 108;
}

FleetManager::~FleetManager() 
{
    for (TacticalUnit* unit : fleet) {
        delete unit;
    }
    fleet.clear();
}

void FleetManager::showFleet()
{
    clearScreen();
    if (fleet.empty()) {
        cout << "Your fleet is empty!\n";
    }
    else {
        cout << "This is your fleet:\n";
        for_each(fleet.begin(), fleet.end(), [](TacticalUnit* unit) {
            cout << *unit << "\n";
            });
    }
    cout << "\nPress any key to go back\n";
    string input;
    getline(cin, input);
}

void FleetManager::addUnit()
{
    clearScreen();
    cout << "What type of Drone would you like to add?\n";
    cout << "Your balance is " << balance << ".\n";
    cout << "1. CombatDrone  \"Ares\"   - 100 coins\n";
    cout << "2. ScoutDrone   \"Seeker\" - 80 coins\n";
    cout << "3. ChargerDrone \"Helper\" - 60 coins\n";
    cout << "4. Cancel\n<< ";

    string line;
    getline(cin, line);
    if (line.empty()) return;

    int choice = 0;
    try { choice = stoi(line); }
    catch (...) { return; }

    try {
        if (choice == 1) {
            if (balance < 100) throw InsufficientFundsException(balance, 100);
            fleet.push_back(new CombatDrone(nextId++));
            balance -= 100;
            cout << "CombatDrone \"Ares\" [ID: " << (nextId - 1) << "] added!\n";
        }
        else if (choice == 2) {
            if (balance < 80) throw InsufficientFundsException(balance, 80);
            fleet.push_back(new ScoutDrone(nextId++));
            balance -= 80;
            cout << "ScoutDrone \"Seeker\" [ID: " << (nextId - 1) << "] added!\n";
        }
        else if (choice == 3) {
            if (balance < 60) throw InsufficientFundsException(balance, 60);
            fleet.push_back(new ChargerDrone(nextId++));
            balance -= 60;
            cout << "ChargerDrone \"Helper\" [ID: " << (nextId - 1) << "] added!\n";
        }
    }
    catch (const InsufficientFundsException& e) {
        cout << e.what() << "\n";
    }
    pause();
}

void FleetManager::deployMission()
{
}
void FleetManager::upgradeCenter() {}
void FleetManager::saveFleet() {}
void FleetManager::loadFleet() {}

void FleetManager::pause()
{
    cout << "\nPress ENTER to continue...";
    cin.ignore(1000, '\n');
    cin.get();
}

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