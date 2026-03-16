#include "ChargerDrone.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <random>
using namespace std;

static mt19937& rng() {
    static mt19937 engine(random_device{}());
    return engine;
}

ChargerDrone::ChargerDrone(int id, int battery, int powerBank)
    : TacticalUnit(id, "Helper", battery), powerBank(powerBank) {
}

void ChargerDrone::performAction(BattleContext& ctx) {
    cout << "  >> "; printStats(cout); cout << "\n";

    cout << "[SUPPORT] " << name << " (ID: " << id << ") charges an ally.\n";

    if (!ctx.fleet) { setBattery(battery - 5); return; }

    TacticalUnit* target = nullptr;
    int minBat = INT_MAX;
    for (TacticalUnit* ally : *ctx.fleet) {
        if (ally == this) continue;
        if (ally->getBattery() < minBat) {
            minBat = ally->getBattery();
            target = ally;
        }
    }

    if (target) {
        uniform_int_distribution<int> dist(0, 30);
        int transfer = min(dist(rng()), powerBank);
        if (transfer > 0) {
            target->setBattery(target->getBattery() + transfer);
            cout << "  -> Transfers " << transfer << " energy to "
                << target->getName() << " (ID: " << target->getId()
                << "). Battery: " << target->getBattery() << "%\n";
        }
    }

    setBattery(battery - 5);
}

void ChargerDrone::printStats(ostream& out) const {
    out << "[ID: " << id << "] ChargerDrone \"" << name
        << "\" | Battery: " << battery
        << "% | Power bank - " << powerBank << "%";
}

TacticalUnit& ChargerDrone::operator+=(const UpgradeModule& mod) {
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

string ChargerDrone::getType() const noexcept { return "ChargerDrone"; }
string ChargerDrone::serialize() const {
    return "ChargerDrone|" + to_string(id) + "|"
        + to_string(battery) + "|" + to_string(powerBank);
}

void ChargerDrone::setPowerBank(int val)
{
    powerBank = val;
}

int ChargerDrone::getPowerBank() const noexcept { return powerBank; }