#pragma once
#include <string>

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
    std::string label;
    UpgradeType type;
    int cost;

public:
    UpgradeModule(const std::string& label, UpgradeType type, int cost);

    std::string getLabel() const noexcept;
    UpgradeType getType() const noexcept;
    int getCost() const noexcept;
};
