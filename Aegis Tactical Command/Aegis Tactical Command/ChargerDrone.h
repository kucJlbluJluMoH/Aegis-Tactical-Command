#pragma once
#include "TacticalUnit.h"
#include "UpgradeModule.h"

class ChargerDrone : public TacticalUnit {
private:
    int powerBank;

public:
    ChargerDrone(int id, int battery = 100, int powerBank = 100);

    void performAction(BattleContext& ctx) override;
    void printStats(ostream& out) const override;
    TacticalUnit& operator+(const UpgradeModule& mod) override;

    string getType() const override;
    string serialize() const override;

    int getPowerBank() const;
};