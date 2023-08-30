#include "dynamicdata.h"

DynamicData::DynamicData()
{

}

int DynamicData::getFreeId(){
    if (!freeIds.size()){
        maxID++;
        return maxID;
    }
    else{
        int id = freeIds.front();
        freeIds.pop();
        return id;
    }
}

void DynamicData::addFreeId(int id){
    freeIds.push(id);
}

void DynamicData::setIdGen(int id, int gen){
    IdGens[id] = gen;

}
void DynamicData::delIdGen(int id){
    IdGens.erase(id);

}
int DynamicData::getIdGen(int id){
    return IdGens[id];

}

void DynamicData::addBorn(BornData data){
    _bornData.push_back(data);
}

std::vector<BornData> DynamicData::getBorns(){
    std::vector<BornData> data;
    std::copy(_bornData.begin(), _bornData.end(), back_inserter(data));
    _bornData.clear();
    return data;
}

DynamicData::~DynamicData()
{
    //dtor
}
