#include "ScoutDrone.h"
#include "CombatDrone.h"
#include <algorithm>
#include <iostream>
using namespace std;

ScoutDrone::ScoutDrone(int id, int battery, int affectedUnits, double accuracyMult)
    : TacticalUnit(id, "Seeker", battery),
    affectedUnits(affectedUnits), accuracyMultiplier(accuracyMult) {
}

void ScoutDrone::performAction(BattleContext& ctx) {
    cout << "  >> ScoutDrone [ID: " << id << "] \"" << name
        << "\" | Battery: " << battery << "% | Affected units: " << affectedUnits
        << " | Acc.Mult: +" << static_cast<int>((accuracyMultiplier - 1.0) * 100) << "%\n";

    cout << "[SUPPORT] " << name << " (ID: " << id
        << ") scans the enemy! Accuracy bonus +"
        << static_cast<int>((accuracyMultiplier - 1.0) * 100) << "%.\n";

    if (!ctx.fleet) { setBattery(battery - 5); return; }

    vector<CombatDrone*> combatants;
    for (TacticalUnit* u : *ctx.fleet) {
        if (u->getBattery() > 0) {
            CombatDrone* cd = dynamic_cast<CombatDrone*>(u);
            if (cd) combatants.push_back(cd);
        }
    }

    for (int i = (int)combatants.size() - 1; i > 0; --i)
        swap(combatants[i], combatants[rand() % (i + 1)]);

    int count = min(affectedUnits, (int)combatants.size());
    for (int i = 0; i < count; ++i) {
        combatants[i]->addAccuracyBonus(accuracyMultiplier - 1.0);
        cout << "  -> Buffs CombatDrone [ID: " << combatants[i]->getId()
            << "] (+" << static_cast<int>((accuracyMultiplier - 1.0) * 100) << "% acc)\n";
    }

    setBattery(battery - 5);
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

string ScoutDrone::getType()   const { return "ScoutDrone"; }
string ScoutDrone::serialize() const {
    return "ScoutDrone|" + to_string(id) + "|" + to_string(battery)
        + "|" + to_string(affectedUnits) + "|" + to_string(accuracyMultiplier);
}
int    ScoutDrone::getAffectedUnits()     const { return affectedUnits; }
double ScoutDrone::getAccuracyMultiplier() const { return accuracyMultiplier; }