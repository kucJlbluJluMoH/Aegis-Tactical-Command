#pragma once
#include "TacticalUnit.h"
#include "UpgradeModule.h"

class CombatDrone : public TacticalUnit {
private:
    int damage;
    int accuracy;
public:
    CombatDrone(int id, int battery = 100, int damage = 30,
        int accuracy = 70);

    void performAction(double accuracyMult) override;
    void printStats(ostream& out) const override;
    TacticalUnit& operator+(const UpgradeModule& mod) override;

    int fireAndGetDamage(double accuracyMult);

    string getType() const override;
    string serialize() const override;
    
    int getDamage() const;
    int getAccuracy() const;
};