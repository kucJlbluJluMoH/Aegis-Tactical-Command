#pragma once
#include <string>
using namespace std;

enum class UpgradeType {
    DAMAGE,
    ACCURACY,
    BATTERY,
    AFFECTED_UNITS,
    ACCURACY_MULT,
    POWER_BANK
};

class UpgradeModule {
private:
    string label;
    UpgradeType type;
    int cost;

public:
    UpgradeModule(const string& label, UpgradeType type, int cost);

    string getLabel() const;
    UpgradeType getType() const;
    int getCost() const;
};
