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
    void printStats(ostream& out) const override;
    TacticalUnit& operator+(const UpgradeModule& mod) override;

    string getType() const override;
    string serialize() const override;

    int getAffectedUnits() const;
    void setAffectedUnits(int val);
    void setAccuracyMultiplier(int val);
    double getAccuracyMultiplier() const;
};