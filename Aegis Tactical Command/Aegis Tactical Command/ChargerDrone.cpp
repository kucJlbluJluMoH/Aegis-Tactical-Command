#include "ChargerDrone.h"
#include <algorithm>
#include <iostream>
using namespace std;

ChargerDrone::ChargerDrone(int id, int battery, int powerBank)
    : TacticalUnit(id, "Helper", battery), powerBank(powerBank) {
}

void ChargerDrone::performAction(double /*accuracyMult*/) {
    cout << "[SUPPORT] " << name << " (ID: " << id
        << ") is ready to recharge an ally.\n";
}

void ChargerDrone::printStats(ostream& out) const {
    out << "[ID: " << id << "] ChargerDrone \"" << name
        << "\" | Battery: " << battery
        << "% | Power bank - " << powerBank << "%";
}

TacticalUnit& ChargerDrone::operator+(const UpgradeModule& mod) {
    switch (mod.getType()) {
    case UpgradeType::POWER_BANK:
        powerBank += 20;
        break;
    case UpgradeType::BATTERY:
        maxBattery += 20;
        battery = min(battery, maxBattery);
        break;
    default:
        cout << "[WARN] This upgrade does not apply to ChargerDrone.\n";
    }
    return *this;
}

string ChargerDrone::getType() const { return "ChargerDrone"; }

string ChargerDrone::serialize() const {
    return "ChargerDrone|" + to_string(id) + "|"
        + to_string(battery) + "|" + to_string(powerBank);
}

int ChargerDrone::getPowerBank() const { return powerBank;}