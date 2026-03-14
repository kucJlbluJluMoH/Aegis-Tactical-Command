#include "FleetManager.h"
#include <algorithm>
#include <iostream>
using namespace std;

FleetManager::FleetManager() : balance(1000), nextId(100), saveFileName("") 
{
    srand(static_cast<unsigned>(time(nullptr)));
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

void FleetManager::upgradeCenter() 
{
    clearScreen();
    cout << "Enter the ID of a drone you'd like to upgrade:\n";
    cout << "1. Cancel\n<< ";

    string line;
    getline(cin, line);
    if (line == "1" || line.empty()) return;

    int targetId = 0;
    try { targetId = stoi(line); }
    catch (...) { return; }

    auto it = find_if(fleet.begin(), fleet.end(),
        [targetId](TacticalUnit* u) { return u->getId() == targetId; });

    if (it == fleet.end()) {
        cout << "No drone found with ID " << targetId << ".\n";
        pause();
        return;
    }

    TacticalUnit* drone = *it;
    cout << "Selected drone: " << *drone << "\n";
    cout << "Your balance is: " << balance << "\n";
    cout << "Select an upgrade you want to install:\n";

    vector<UpgradeModule> options;
    const string type = drone->getType();

    if (type == "CombatDrone") {
        CombatDrone* cd = dynamic_cast<CombatDrone*>(drone);
        options.push_back(UpgradeModule(
            "Damage Upgrade: " + to_string(cd->getDamage()) + " -> " + to_string(cd->getDamage() + 10),
            UpgradeType::DAMAGE, 30));
        options.push_back(UpgradeModule(
            "Accuracy Upgrade: " + to_string(cd->getAccuracy()) + " -> " + to_string(cd->getAccuracy() + 10),
            UpgradeType::ACCURACY, 30));
        options.push_back(UpgradeModule(
            "Battery Upgrade: " + to_string(drone->getMaxBattery()) + " -> " + to_string(drone->getMaxBattery() + 20),
            UpgradeType::BATTERY, 25));

    }
    else if (type == "ScoutDrone") {
        ScoutDrone* sd = dynamic_cast<ScoutDrone*>(drone);
        options.push_back(UpgradeModule(
            "Affected units: " + to_string(sd->getAffectedUnits()) + " -> " + to_string(sd->getAffectedUnits() + 1),
            UpgradeType::AFFECTED_UNITS, 25));
        options.push_back(UpgradeModule(
            "Accuracy multiplier: " + to_string(sd->getAccuracyMultiplier()).substr(0, 3)
            + " -> " + to_string(sd->getAccuracyMultiplier() + 0.1).substr(0, 3),
            UpgradeType::ACCURACY_MULT, 25));
        options.push_back(UpgradeModule(
            "Battery Upgrade: " + to_string(drone->getMaxBattery()) + " -> " + to_string(drone->getMaxBattery() + 20),
            UpgradeType::BATTERY, 25));

    }
    else if (type == "ChargerDrone") {
        ChargerDrone* chd = dynamic_cast<ChargerDrone*>(drone);
        options.push_back(UpgradeModule(
            "Power bank Upgrade: " + to_string(chd->getPowerBank()) + " -> " + to_string(chd->getPowerBank() + 20),
            UpgradeType::POWER_BANK, 30));
        options.push_back(UpgradeModule(
            "Battery Upgrade: " + to_string(drone->getMaxBattery()) + " -> " + to_string(drone->getMaxBattery() + 20),
            UpgradeType::BATTERY, 25));
    }

    for (int i = 0; i < (int)options.size(); i++) {
        cout << (i + 1) << ". " << options[i].getLabel()
            << "  (" << options[i].getCost() << " coins)\n";
    }
    cout << (options.size() + 1) << ". Cancel\n<< ";

    string sel;
    getline(cin, sel);
    int selIdx = 0;
    try { selIdx = stoi(sel) - 1; }
    catch (...) { pause(); return; }

    if (selIdx < 0 || selIdx >= (int)options.size()) { pause(); return; }

    UpgradeModule& chosen = options[selIdx];
    try {
        if (balance < chosen.getCost())
            throw InsufficientFundsException(balance, chosen.getCost());
        *drone + chosen;
        balance -= chosen.getCost();
        cout << "Upgrade applied! New stats: " << *drone << "\n";
    }
    catch (const InsufficientFundsException& e) {
        cout << e.what() << "\n";
    }
    pause();
}

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

        sort(fleet.begin(), fleet.end(),
            [](TacticalUnit* a, TacticalUnit* b) {
                return a->getId() < b->getId();
            });


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