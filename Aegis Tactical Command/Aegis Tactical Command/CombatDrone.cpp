#include "CombatDrone.h"
#include <algorithm>
#include <iostream>
using namespace std;

CombatDrone::CombatDrone(int id, int battery, int damage, int accuracy)
    : TacticalUnit(id, "Ares", battery),
    damage(damage), accuracy(accuracy) {
}

void CombatDrone::performAction(double accuracyMult) {
    int effectiveAcc = static_cast<int>(accuracy * accuracyMult);
    bool hit = (rand() % 100) < effectiveAcc;
    bool crit = hit && (rand() % 100) < 30;

    if (hit) {
        int dmg = crit ? static_cast<int>(damage * 1.5) : damage;
        cout << "[ACTION] " << name << " (ID: " << id << ") fires! Damage: "
            << dmg << (crit ? " (Crit!)" : "") << ".\n";
    }
    else {
        cout << "[ACTION] " << name << " (ID: " << id
            << ") fires but misses!\n";
    }
}

int CombatDrone::fireAndGetDamage(double accuracyMult) {
    int effectiveAcc = static_cast<int>(accuracy * accuracyMult);
    bool hit = (rand() % 100) < effectiveAcc;
    bool crit = hit && (rand() % 100) < 30;
    if (hit) return crit ? static_cast<int>(damage * 1.5) : damage;
    return 0;
}

void CombatDrone::printStats(ostream& out) const {
    out << "[ID: " << id << "] CombatDrone \"" << name
        << "\" | Battery: " << battery << "% | Damage: " << damage
        << " | Accuracy: " << accuracy;
}

TacticalUnit& CombatDrone::operator+(const UpgradeModule& mod) {
    switch (mod.getType()) {
    case UpgradeType::DAMAGE:
        damage += 10;
        break;
    case UpgradeType::ACCURACY:
        accuracy += 10;
        accuracy = min(accuracy, 100);
        break;
    case UpgradeType::BATTERY:
        maxBattery += 20;
        battery = min(battery, maxBattery);
        break;
    default:
        cout << "[WARN] This upgrade does not apply to CombatDrone.\n";
    }
    return *this;
}

string CombatDrone::getType() const { return "CombatDrone"; }

string CombatDrone::serialize() const {
    return "CombatDrone|" + to_string(id) + "|" + to_string(battery)
        + "|" + to_string(damage) + "|" + to_string(accuracy);
}

int CombatDrone::getDamage()   const { return damage; }
int CombatDrone::getAccuracy() const { return accuracy; }