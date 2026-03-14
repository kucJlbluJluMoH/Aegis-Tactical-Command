#include "ScoutDrone.h"
#include <algorithm>
#include <iostream>
using namespace std;

ScoutDrone::ScoutDrone(int id, int battery, int affectedUnits, double accuracyMult)
    : TacticalUnit(id, "Seeker", battery),
    affectedUnits(affectedUnits), accuracyMultiplier(accuracyMult) {
}

void ScoutDrone::performAction( double /*accuracyMult*/) {
    cout << "[SUPPORT] " << name << " (ID: " << id
        << ") scans the enemy! Accuracy increased by "
        << static_cast<int>((accuracyMultiplier - 1.0) * 100) << "%.\n";
}

void ScoutDrone::printStats(ostream& out) const {
    out << "[ID: " << id << "] ScoutDrone \"" << name
        << "\" | Battery: " << battery
        << "% | Affected units: " << affectedUnits
        << " | Accuracy multiplier: " << accuracyMultiplier;
}

TacticalUnit& ScoutDrone::operator+(const UpgradeModule& mod) {
    switch (mod.getType()) {
    case UpgradeType::AFFECTED_UNITS:
        affectedUnits += 1;
        break;
    case UpgradeType::ACCURACY_MULT:
        accuracyMultiplier += 0.1;
        break;
    case UpgradeType::BATTERY:
        maxBattery += 20;
        battery = min(battery, maxBattery);
        break;
    default:
        cout << "[WARN] This upgrade does not apply to ScoutDrone.\n";
    }
    return *this;
}

string ScoutDrone::getType() const { return "ScoutDrone"; }

string ScoutDrone::serialize() const {
    return "ScoutDrone|" + to_string(id) + "|" + to_string(battery)
        + "|" + to_string(affectedUnits)
        + "|" + to_string(accuracyMultiplier);
}

int ScoutDrone::getAffectedUnits() const { return affectedUnits; }
double ScoutDrone::getAccuracyMultiplier() const { return accuracyMultiplier; }