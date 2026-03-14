#include "CombatDrone.h"
#include <algorithm>
#include <iostream>
using namespace std;

CombatDrone::CombatDrone(int id, int battery, int damage, int accuracy)
    : TacticalUnit(id, "Ares", battery),
    damage(damage), accuracy(accuracy), pendingAccMult(1.0), lastDamage(0) {
}

void CombatDrone::addAccuracyBonus(double bonus) {
    pendingAccMult += bonus;
}

void CombatDrone::resetAccuracyBonus() {
    pendingAccMult = 1.0;
}

void CombatDrone::performAction(BattleContext&) {
    cout << "  >> CombatDrone [ID: " << id << "] \"" << name
        << "\" | Battery: " << battery << "% | Damage: " << damage
        << " | Accuracy: " << accuracy
        << " | Acc.Mult: x" << static_cast<int>(pendingAccMult * 100) / 100.0 << "\n";

    int effectiveAcc = static_cast<int>(accuracy * pendingAccMult);
    bool hit = (rand() % 100) < effectiveAcc;
    bool crit = hit && (rand() % 100) < 30;

    if (hit) {
        lastDamage = crit ? static_cast<int>(damage * 1.5) : damage;
        cout << "[ACTION] " << name << " (ID: " << id << ") fires! Damage: "
            << lastDamage << (crit ? " (Crit!)" : "") << ".\n";
    }
    else {
        lastDamage = 0;
        cout << "[ACTION] " << name << " (ID: " << id << ") fires but misses!\n";
    }
    setBattery(battery - 5);
}

int CombatDrone::getLastDamage() const { return lastDamage; }

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
        accuracy = min(accuracy + 10, 100);
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

string CombatDrone::getType()  const { return "CombatDrone"; }
string CombatDrone::serialize() const {
    return "CombatDrone|" + to_string(id) + "|" + to_string(battery)
        + "|" + to_string(damage) + "|" + to_string(accuracy);
}
int CombatDrone::getDamage()   const { return damage; }
int CombatDrone::getAccuracy() const { return accuracy; }