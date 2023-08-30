#ifndef DYNAMICDATA_H
#define DYNAMICDATA_H
#include <queue>
#include <map>
#include <vector>
#include "MyStructs.h"


class DynamicData
{
    public:
        DynamicData();
        virtual ~DynamicData();

        int getFreeId();
        void addFreeId(int id);
        bool isPlay = false;
        void setIdGen(int id, int gen);
        void delIdGen(int id);
        int getIdGen(int id);
        void addBorn(BornData data);
        std::vector<BornData> getBorns();

    protected:

    private:
        std::queue<int> freeIds;
        int maxID = -1;
        std::map<int, int> IdGens;
        std::vector<BornData> _bornData;
};

#endif // DYNAMICDATA_H
