#pragma once
#include "TacticalUnit.h"
#include "UpgradeModule.h"

class ScoutDrone : public TacticalUnit {
private:
    int affectedUnits;
    double accuracyMultiplier;

public:
    ScoutDrone(int id, int battery = 100, int affectedUnits = 3,
        double accuracyMult = 1.1);

    void performAction(BattleContext& ctx) override;
    void printStats(std::ostream& out) const override;
    TacticalUnit& operator+=(const UpgradeModule& mod) override;

    std::string getType() const noexcept override;
    std::string serialize() const override;

    int getAffectedUnits() const noexcept;
    double getAccuracyMultiplier() const noexcept;

    void setAffectedUnits(int val);
    void setAccuracyMultiplier(double val);
};
