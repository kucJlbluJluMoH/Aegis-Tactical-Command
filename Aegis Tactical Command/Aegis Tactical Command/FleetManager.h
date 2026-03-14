#pragma once
#include <vector>
#include <string>
#include "TacticalUnit.h"
using namespace std;

class FleetManager
{
private:
    vector<TacticalUnit*> fleet;

    int nextId;
    int balance;
    string saveFileName;

    void clearScreen();
    void showFleet();
    void addUnit();
    void upgradeCenter();
    void deployMission();
    void saveFleet();
    void loadFleet();

public:
    FleetManager();
    ~FleetManager();

    void run();
};
