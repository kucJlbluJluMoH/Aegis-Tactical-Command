#pragma once
#include <vector>
#include <string>
#include "TacticalUnit.h"
#include "UpgradeModule.h"
#include "CombatDrone.h"
#include "ScoutDrone.h"
#include "ChargerDrone.h"
#include "Exceptions.h"

class FleetManager
{
private:
    std::vector<TacticalUnit*> fleet;

    int nextId;
    int balance;
    std::string saveFileName;

    void pause();
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
