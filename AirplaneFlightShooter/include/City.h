#ifndef CITY_H
#define CITY_H

#include "Config.h"
#include "Entities.h"

extern Building buildings[BUILDING_COUNT];
extern Tree trees[TREE_COUNT];

bool isOnRoad(float px, float pz);
void initBuildings();
void initTrees();

#endif
