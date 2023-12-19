#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <stdlib.h>
#include <mutex>
#include "field.h"
#include <cmath>

extern Color backgroundColor;
extern int countFrames;
extern int countTicks;
extern int FPS;
extern int TPS;
extern std::vector<int> StandartNeiron1, StandartNeiron2;
extern double kEnergyGrow, kMutate, kPhotosintes, kPredotor, kGo, deltMutate, kMultiply, kBornEnergy, kGrow;
extern int kFind, kEnemy, kMaxNeighbour;
extern int WorldWidth, WorldHeight, year;
extern Rect WorldRect;

extern DynamicData dd;

extern zone* BaseSettings;

extern Field field;

double getRandNum(double minNum, double maxNum, double step);
