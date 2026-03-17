#pragma once
#include <iostream>
#include <string>
#include <vector>

class UpgradeModule;
class TacticalUnit;

struct BattleContext {
    std::vector<TacticalUnit*>* fleet = nullptr;
};

class TacticalUnit {
protected:
    int id;
    std::string name;
    int battery;
    int maxBattery;

public:
    TacticalUnit(int id, const std::string& name, int battery = 100);

    virtual ~TacticalUnit();

    virtual void performAction(BattleContext& ctx) = 0;
    virtual void printStats(std::ostream& out) const = 0;

    friend std::ostream& operator<<(std::ostream& out, const TacticalUnit& unit) {
        unit.printStats(out);
        return out;
    }

    virtual TacticalUnit& operator+=(const UpgradeModule& mod) = 0;

    virtual std::string getType() const noexcept = 0;
    virtual std::string serialize() const = 0;

    int getId() const noexcept;
    std::string getName() const noexcept;
    int getBattery() const noexcept;
    int getMaxBattery() const noexcept;

    void setBattery(int b);
    void setMaxBattery(int m);
};
