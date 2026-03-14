#include "TacticalUnit.h"
#include <algorithm>
using namespace std;

TacticalUnit::TacticalUnit(int id, const string& name, int battery)
    : id(id), name(name), battery(battery), maxBattery(battery) {
}

TacticalUnit::~TacticalUnit() 
{
}

int TacticalUnit::getId() const { return id; }
string TacticalUnit::getName() const { return name; }
int TacticalUnit::getBattery() const { return battery; }
int TacticalUnit::getMaxBattery() const { return maxBattery; }

void TacticalUnit::setBattery(int b) {
    battery = min(b, maxBattery);
    if (battery < 0) battery = 0;
}

void TacticalUnit::setMaxBattery(int m) {
    maxBattery = m;
}