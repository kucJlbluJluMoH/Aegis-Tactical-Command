#include "CombatDrone.h"
#include <algorithm>
#include <iostream>
#include <random>
using namespace std;

static mt19937& rng() {
    static mt19937 engine(random_device{}());
    return engine;
}

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
    cout << "  >> "; printStats(cout);
    cout << " | Acc.Mult: x" << static_cast<int>(pendingAccMult * 100) / 100.0 << "\n";

    uniform_int_distribution<int> d100(0, 99);
    int effectiveAcc = static_cast<int>(accuracy * pendingAccMult);
    bool hit = d100(rng()) < effectiveAcc;
    bool crit = hit && d100(rng()) < 30;

    cout << "[ACTION] " << name << " (ID: " << id << ") fires!";
    if (hit) {
        lastDamage = crit ? static_cast<int>(damage * 1.5) : damage;
        cout << " Target hit! Dealing " << lastDamage << " damage"
            << (crit ? " (CRITICAL!)" : "") << ".\n";
    }
    else {
        lastDamage = 0;
        cout << " Target missed.\n";
    }
    setBattery(battery - 5);
}

void CombatDrone::setDamage(int dmg)
{
    damage = dmg;
}

void CombatDrone::setAccuracy(int acc)
{
    accuracy = acc;
}

int CombatDrone::getLastDamage() const noexcept { return lastDamage; }

void CombatDrone::printStats(ostream& out) const {
    out << "[ID: " << id << "] CombatDrone \"" << name
        << "\" | Battery: " << battery << "% | Damage: " << damage
        << " | Accuracy: " << accuracy;
}

TacticalUnit& CombatDrone::operator+=(const UpgradeModule& mod) {
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

string CombatDrone::getType() const noexcept { return "CombatDrone"; }
string CombatDrone::serialize() const {
    return "CombatDrone|" + to_string(id) + "|" + to_string(battery)
        + "|" + to_string(damage) + "|" + to_string(accuracy);
}
int CombatDrone::getDamage() const noexcept { return damage; }
int CombatDrone::getAccuracy() const noexcept { return accuracy; }