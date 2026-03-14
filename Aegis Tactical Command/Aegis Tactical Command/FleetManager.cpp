#include "FleetManager.h"
#include <algorithm>
#include <iostream>
#include <climits>
#include <fstream>
#include <filesystem>
#include <random>
using namespace std;
namespace fs = filesystem;

FleetManager::FleetManager() : nextId(100), balance(500), saveFileName("")
{
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
    clearScreen();

    if (fleet.empty()) {
        cout << "Your fleet is empty! Add some drones before deploying.\n";
        pause();
        return;
    }

    cout << "Choose a mission target:\n";
    cout << "1. Abandoned Factory\n";
    cout << "2. Supply Base\n";
    cout << "3. Cancel\n<< ";
    string line;
    getline(cin, line);
    int missionChoice = 0;
    try { missionChoice = stoi(line); }
    catch (...) {}
    if (missionChoice == 3 || missionChoice == 0) return;

    string missionName = (missionChoice == 1) ? "Abandoned Factory" : "Supply Base";

    clearScreen();
    cout << "========================================\n";
    cout << "  MISSION: " << missionName << "\n";
    cout << "  Boss: GOLIATH | HP: 1000 | Turns: 10\n";
    cout << "========================================\n";
    cout << "Press ENTER to start...\n";
    cin.get();

    int bossHp = 1000;
    const int MAX_TURNS = 10;

    for (int turn = 1; turn <= MAX_TURNS && bossHp > 0; ++turn) {
        int aliveCheck = 0;
        for (TacticalUnit* u : fleet)
            if (u->getBattery() > 0) ++aliveCheck;
        if (aliveCheck == 0) {
            clearScreen();
            cout << "========================================\n";
            cout << "  DEFEAT! All drones are out of power!\n";
            cout << "  Consolation reward: +10 coins\n";
            cout << "========================================\n";
            balance += 10;
            for (TacticalUnit* u : fleet)
                u->setBattery(u->getMaxBattery());
            pause();
            return;
        }

        clearScreen();
        cout << "========================================\n";
        cout << "  TURN " << turn << " / " << MAX_TURNS << "\n";
        cout << "  Boss HP: " << bossHp << "\n";
        cout << "========================================\n\n";

        for (TacticalUnit* u : fleet) {
            CombatDrone* cd = dynamic_cast<CombatDrone*>(u);
            if (cd) cd->resetAccuracyBonus();
        }

        cout << "--- [SCOUT PHASE] ---\n";
        {
            BattleContext ctx;
            ctx.fleet = &fleet;
            for (TacticalUnit* u : fleet) {
                if (u->getBattery() > 0 && u->getType() == "ScoutDrone")
                    u->performAction(ctx);
            }
        }
        cout << "\n";

        cout << "--- [COMBAT PHASE] ---\n";
        for (TacticalUnit* u : fleet) {
            if (u->getBattery() <= 0 || u->getType() != "CombatDrone") continue;

            if (u->getBattery() < 10 && (rand() % 100) < 30) {
                try {
                    throw BatteryException(
                        u->getName() + " [ID:" + to_string(u->getId()) + "]",
                        u->getBattery());
                }
                catch (const BatteryException& e) {
                    cout << e.what() << " Skipping turn.\n";
                    continue;
                }
            }

            BattleContext ctx;
            u->performAction(ctx);

            CombatDrone* cd = dynamic_cast<CombatDrone*>(u);
            if (cd) bossHp = max(0, bossHp - cd->getLastDamage());

            if (bossHp == 0) break;
        }
        cout << "\n";

        if (bossHp == 0) break;

        cout << "--- [CHARGER PHASE] ---\n";
        {
            BattleContext ctx;
            ctx.fleet = &fleet;
            for (TacticalUnit* u : fleet) {
                if (u->getBattery() > 0 && u->getType() == "ChargerDrone")
                    u->performAction(ctx);
            }
        }
        cout << "\n";

        cout << "--- [ENEMY ATTACK: GOLIATH] ---\n";
        vector<TacticalUnit*> byBattery = fleet;
        sort(byBattery.begin(), byBattery.end(),
            [](TacticalUnit* a, TacticalUnit* b) {
                return a->getBattery() > b->getBattery();
            });

        int targets = min(2, (int)byBattery.size());
        for (int i = 0; i < targets; ++i) {
            byBattery[i]->setBattery(byBattery[i]->getBattery() - 20);
            cout << "[ENEMY] Goliath strikes " << byBattery[i]->getName()
                << " (ID: " << byBattery[i]->getId()
                << ")! Battery: " << byBattery[i]->getBattery() << "%\n";
        }
        cout << "\n";

        int alive = 0, totalBat = 0;
        for (TacticalUnit* u : fleet) {
            if (u->getBattery() > 0) { ++alive; totalBat += u->getBattery(); }
        }
        int avgBat = alive > 0 ? totalBat / alive : 0;

        cout << "--- [TURN " << turn << " REPORT] ---\n";
        cout << "  Boss HP:       " << bossHp << "\n";
        cout << "  Active drones: " << alive << " / " << fleet.size() << "\n";
        cout << "  Avg battery:   " << avgBat << "%\n";
        cout << "\n  [FLEET STATUS]\n";
        for (TacticalUnit* u : fleet) {
            cout << "  " << *u;
            if (u->getBattery() <= 0) cout << " [OFFLINE]";
            cout << "\n";
        }
        cout << "\nPress ENTER for next turn...\n";
        cin.get();
    }

    clearScreen();
    if (bossHp <= 0) {
        cout << "========================================\n";
        cout << "  VICTORY! Goliath has been destroyed!\n";
        cout << "  Reward: +50 coins\n";
        cout << "========================================\n";
        balance += 50;
    }
    else {
        cout << "========================================\n";
        cout << "  DEFEAT! Goliath survived with " << bossHp << " HP.\n";
        cout << "  Consolation reward: +10 coins\n";
        cout << "========================================\n";
        balance += 10;
    }

    for (TacticalUnit* u : fleet)
        u->setBattery(u->getMaxBattery());

    pause();
}

void FleetManager::upgradeCenter()
{
    clearScreen();
    cout << "Enter the ID of a drone you'd like to upgrade:\n";
    if (fleet.empty()) {
        cout << "Your fleet is empty!\n";
        pause();
        return;
    }
    else {
        cout << "This is your fleet:\n";
        for_each(fleet.begin(), fleet.end(), [](TacticalUnit* unit) {
            cout << *unit << "\n";
            });
    }
    cout << "1. Cancel\n<< ";

    string line;
    getline(cin, line);
    if (line == "1" || line.empty()) return;

    int targetId = 0;
    try { targetId = stoi(line); }
    catch (...) { return; }

    try {
        auto it = find_if(fleet.begin(), fleet.end(),
            [targetId](TacticalUnit* u) { return u->getId() == targetId; });

        if (it == fleet.end())
            throw InvalidDroneIdException(targetId);

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
            *drone += chosen;
            balance -= chosen.getCost();
            cout << "Upgrade applied! New stats: " << *drone << "\n";
        }
        catch (const InsufficientFundsException& e) {
            cout << e.what() << "\n";
        }
    }
    catch (const InvalidDroneIdException& e) {
        cout << e.what() << "\n";
    }

    pause();
}

void FleetManager::saveFleet()
{
    clearScreen();
    cout << "========================================\n";
    cout << "              SAVE FLEET\n";
    cout << "========================================\n";
    cout << "Current save file: " << (saveFileName.empty() ? "-" : saveFileName) << "\n";
    cout << "Enter save file name (without extension): ";

    string input;
    getline(cin, input);
    if (input.empty()) {
        cout << "Save cancelled.\n";
        pause();
        return;
    }

    string fileName = input + ".fleet";

    try {
        ofstream out(fileName);
        if (!out.is_open())
            throw FileIOException("Cannot open file for writing: " + fileName);

        out << nextId << "\n";
        out << balance << "\n";
        out << fleet.size() << "\n";

        for (TacticalUnit* u : fleet) {
            string type = u->getType();
            out << type << "\n";
            out << u->getId() << "\n";
            out << u->getBattery() << "\n";
            out << u->getMaxBattery() << "\n";

            if (type == "CombatDrone") {
                CombatDrone* cd = static_cast<CombatDrone*>(u);
                out << cd->getDamage() << "\n";
                out << cd->getAccuracy() << "\n";
            }
            else if (type == "ScoutDrone") {
                ScoutDrone* sd = static_cast<ScoutDrone*>(u);
                out << sd->getAffectedUnits() << "\n";
                out << sd->getAccuracyMultiplier() << "\n";
            }
            else if (type == "ChargerDrone") {
                ChargerDrone* chd = static_cast<ChargerDrone*>(u);
                out << chd->getPowerBank() << "\n";
            }
            // serialize() used as a human-readable record per drone
            out << "# " << u->serialize() << "\n";
        }

        out.close();
        saveFileName = fileName;
        cout << "Fleet saved to \"" << fileName << "\".\n";
    }
    catch (const FileIOException& e) {
        cout << e.what() << "\n";
    }

    pause();
}

void FleetManager::loadFleet()
{
    clearScreen();
    cout << "========================================\n";
    cout << "              LOAD FLEET\n";
    cout << "========================================\n";

    cout << "Available save files:\n";
    bool anyFound = false;
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".fleet") {
            cout << "  - " << entry.path().filename().string() << "\n";
            anyFound = true;
        }
    }
    if (!anyFound) {
        cout << "  (none found)\n";
        pause();
        return;
    }

    cout << "Enter save file name (without extension): ";
    string input;
    getline(cin, input);
    if (input.empty()) {
        cout << "Load cancelled.\n";
        pause();
        return;
    }

    string fileName = input + ".fleet";

    try {
        ifstream in(fileName);
        if (!in.is_open())
            throw FileIOException("Cannot open file for reading: " + fileName);

        int loadedNextId, loadedBalance;
        size_t fleetSize;

        in >> loadedNextId >> loadedBalance >> fleetSize;
        in.ignore();

        vector<TacticalUnit*> loadedFleet;
        bool parseError = false;

        for (size_t i = 0; i < fleetSize && !in.eof(); ++i) {
            string type;
            int id, battery, maxBattery;

            getline(in, type);
            in >> id >> battery >> maxBattery;
            in.ignore();

            TacticalUnit* unit = nullptr;

            if (type == "CombatDrone") {
                int damage, accuracy;
                in >> damage >> accuracy;
                in.ignore();
                in.ignore(1024, '\n'); // skip "# ..." serialize line
                CombatDrone* cd = new CombatDrone(id);
                cd->setDamage(damage);
                cd->setAccuracy(accuracy);
                cd->setMaxBattery(maxBattery);
                cd->setBattery(battery);
                unit = cd;
            }
            else if (type == "ScoutDrone") {
                int affectedUnits;
                double accuracyMult;
                in >> affectedUnits >> accuracyMult;
                in.ignore();
                in.ignore(1024, '\n');
                ScoutDrone* sd = new ScoutDrone(id);
                sd->setAffectedUnits(affectedUnits);
                sd->setAccuracyMultiplier(accuracyMult);
                sd->setMaxBattery(maxBattery);
                sd->setBattery(battery);
                unit = sd;
            }
            else if (type == "ChargerDrone") {
                int powerBank;
                in >> powerBank;
                in.ignore();
                in.ignore(1024, '\n');
                ChargerDrone* chd = new ChargerDrone(id);
                chd->setPowerBank(powerBank);
                chd->setMaxBattery(maxBattery);
                chd->setBattery(battery);
                unit = chd;
            }
            else {
                parseError = true;
                break;
            }

            if (unit) loadedFleet.push_back(unit);
        }

        in.close();

        if (parseError) {
            for (TacticalUnit* u : loadedFleet) delete u;
            throw FileIOException("Save file is corrupted: " + fileName);
        }

        for (TacticalUnit* u : fleet) delete u;
        fleet.clear();

        fleet = loadedFleet;
        nextId = loadedNextId;
        balance = loadedBalance;
        saveFileName = fileName;

        cout << "Fleet loaded from \"" << fileName << "\". "
            << fleet.size() << " drone(s) restored.\n";
    }
    catch (const FileIOException& e) {
        cout << e.what() << "\n";
    }

    pause();
}

void FleetManager::pause()
{
    cout << "\nPress ENTER to continue...";
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
