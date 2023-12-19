#include "Settings.h"
 Color backgroundColor(40,40,40);
 int countFrames = 25;
 int countTicks = 1000;
 int FPS = 1000 / countFrames;
 int TPS = 1000 / countTicks;

 std::vector<int> StandartNeiron1 = {7, 8, 8, 8, 4}, StandartNeiron2 = {6, 8, 8, 8, 2};
 double kEnergyGrow = 0.7, kMutate = 0.5, kPhotosintes = 70, kPredotor = 30, kGo = 0.001, deltMutate = 3, kMultiply = 1, kBornEnergy = 13, kGrow = 0.01;
 int kFind = 0.01, kEnemy = 20, kMaxNeighbour = 10;
int WorldWidth = 1600, WorldHeight = 1000, year = 0;
Rect WorldRect(0, 0, WorldWidth, WorldHeight);

DynamicData dd;

zone* BaseSettings = new zone(100, 100, 100, WorldRect, &dd);

Field field(WorldWidth, WorldHeight, BaseSettings, &dd);

double getRandNum(double minNum, double maxNum, double step){
    if (step <= 0 || (maxNum - minNum) < step){
            std::cout << "error";
        return -1;
    }
    double num = std::rand();
    num = int(num) %  int((maxNum - minNum) / step);
    num = num * step;
    num = num + minNum;
    return num;
}
