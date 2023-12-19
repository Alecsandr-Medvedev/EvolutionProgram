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

void DynamicData::setIdGen(int id, Genom gen){
    IdGens[id] = gen;

}
void DynamicData::delIdGen(int id){
    IdGens.erase(id);

}
Genom DynamicData::getIdGen(int id){
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

void DynamicData::addBornTree(std::vector<long long int> bornTree){
    std::vector<long long int> copyBornTree(bornTree);
    _bornTree.push_back(copyBornTree);
}

void DynamicData::writeBornTree(){
    std::ofstream out;
    out.open("bornTree.txt");
    for (std::vector<long long int> els : _bornTree){
        out << els[0] << ";";
        out << els[1] << ";";
        out << els[2] << ";";
        out << els[3] << ";";
        out << els[4] << ";";
        out << els[5] << "\n";
    }
    out.close();
}

DynamicData::~DynamicData()
{
    //dtor
}
