#include "UpgradeModule.h"

UpgradeModule::UpgradeModule(const string& label, UpgradeType type, int cost)
    : label(label), type(type), cost(cost) {
}

string UpgradeModule::getLabel() const { return label; }
UpgradeType UpgradeModule::getType() const { return type; }
int UpgradeModule::getCost() const { return cost; }