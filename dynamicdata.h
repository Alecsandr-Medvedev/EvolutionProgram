#ifndef DYNAMICDATA_H
#define DYNAMICDATA_H
#include <queue>
#include <map>
#include <vector>
#include <fstream>
#include "MyStructs.h"


class DynamicData
{
    public:
        DynamicData();
        virtual ~DynamicData();

        int getFreeId();
        void addFreeId(int id);
        bool isPlay = false;
        void setIdGen(int id, long long int gen);
        void delIdGen(int id);
        int getIdGen(int id);
        void addBorn(BornData data);
        std::vector<BornData> getBorns();
        void addBornTree(std::vector<long long int> bornTree);
        void writeBornTree();

    protected:

    private:
        std::vector<std::vector<long long int>> _bornTree;
        std::queue<long long int> freeIds;
        int maxID = -1;
        std::map<int, int> IdGens;
        std::vector<BornData> _bornData;
};

#endif // DYNAMICDATA_H
