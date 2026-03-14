#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class UpgradeModule;
class TacticalUnit;

struct BattleContext {
    vector<TacticalUnit*>* fleet = nullptr;
};

class TacticalUnit {
protected:
    int id;
    string name;
    int battery;
    int maxBattery;

public:
    TacticalUnit(int id, const string& name, int battery = 100);

    virtual ~TacticalUnit();

    virtual void performAction(BattleContext& ctx) = 0;
    virtual void printStats(ostream& out) const = 0;

    friend ostream& operator<<(ostream& out, const TacticalUnit& unit) {
        unit.printStats(out);
        return out;
    }

    virtual TacticalUnit& operator+(const UpgradeModule& mod) = 0;

    virtual string getType() const = 0;
    virtual string serialize() const = 0;

    int getId() const;
    string getName() const;
    int getBattery() const;
    int getMaxBattery() const;

    void setBattery(int b);
    void setMaxBattery(int m);
};