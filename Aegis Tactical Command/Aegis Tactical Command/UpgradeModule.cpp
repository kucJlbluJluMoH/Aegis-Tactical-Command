#include "UpgradeModule.h"
using namespace std;

UpgradeModule::UpgradeModule(const string& label, UpgradeType type, int cost)
    : label(label), type(type), cost(cost) {
}

string UpgradeModule::getLabel() const noexcept { return label; }
UpgradeType UpgradeModule::getType() const noexcept { return type; }
int UpgradeModule::getCost() const noexcept { return cost; }
