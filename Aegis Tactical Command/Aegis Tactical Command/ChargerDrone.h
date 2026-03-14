#pragma once
#include "TacticalUnit.h"
#include "UpgradeModule.h"

class ChargerDrone : public TacticalUnit {
private:
    int powerBank;

public:
    ChargerDrone(int id, int battery = 100, int powerBank = 100);

    void performAction(BattleContext& ctx) override;
    void printStats(std::ostream& out) const override;
    TacticalUnit& operator+=(const UpgradeModule& mod) override;

    std::string getType() const noexcept override;
    std::string serialize() const override;

    void setPowerBank(int val);
    int getPowerBank() const noexcept;
};
