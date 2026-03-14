#pragma once
#include "TacticalUnit.h"
#include "UpgradeModule.h"

class CombatDrone : public TacticalUnit {
private:
    int damage;
    int accuracy;
    double pendingAccMult;
    int lastDamage;

public:
    CombatDrone(int id, int battery = 100, int damage = 30, int accuracy = 70);

    void performAction(BattleContext& ctx) override;
    void printStats(std::ostream& out) const override;
    TacticalUnit& operator+=(const UpgradeModule& mod) override;

    void addAccuracyBonus(double bonus);
    void resetAccuracyBonus();
    void setDamage(int damage);
    void setAccuracy(int acc);

    int getLastDamage() const noexcept;

    std::string getType() const noexcept override;
    std::string serialize() const override;

    int getDamage() const noexcept;
    int getAccuracy() const noexcept;
};
