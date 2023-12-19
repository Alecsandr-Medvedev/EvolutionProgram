#include "organism.h"

Organism::Organism(double sizeo, double growthRate, double pointPh, double pointPr, double maxSpeed, Rect rect, DynamicData *dd, NeironNet* neironNet1, NeironNet* neironNet2) : GameObject(rect, 'c', Color(int(2.56 * pointPr), int(2.56 * pointPh), 256 - maxSpeed, 256 - growthRate), dd), _neironNetDo(neironNet1), _neironNetGo(neironNet2){
    _energy = sizeo * kBornEnergy;
    _size = sizeo;
    _growthRate = growthRate;
    _pointPh = pointPh;
    _pointPr = pointPr;
    _maxSpeed = maxSpeed;

    _neironNetDo = neironNet1;
    _neironNetGo = neironNet2;

    _gen.growthRate = growthRate;
    _gen.initial_weights_1 = neironNet1->printWeights();
    _gen.initial_weights_2 = neironNet2->printWeights();
    _gen.maxSpeed = maxSpeed;
    _gen.pointPh = pointPh;
    _gen.pointPr = pointPr;
    inField();

}

void Organism::update(){
    inField();
    if (_size <= _interact){
        die();
    }
    else{
            _size -= _interact;
    _interact = 0;
    growth(_growthRate);
	think();
    }
}

std::vector<std::vector<int>> Organism::detectOrganisms(int xCenter, int yCenter){
    int myId = getID();
    int findZone = _size * kFind;
    Rect rect(xCenter + findZone, yCenter + findZone, findZone * 2, findZone * 2);
    std::vector<int> idsFZ = field.getOrganismes(rect);
    Rect r(getRect());
    std::vector<int> idsNear = field.getOrganismes(r);
    for (int num : idsNear) {
        idsFZ.erase(std::remove(idsFZ.begin(), idsFZ.end(), num), idsFZ.end());
    }
    std::vector<int> idsFZ_e = {}, idsNear_e = {}, idsFZ_f = {}, idsNear_f = {};
    for (int el : idsFZ){
        if (el == myId) continue;
        Genom g = dd.getIdGen(el);
        if(compareGens(g)){
            idsFZ_f.push_back(el);
        }
        idsFZ_e.push_back(el);
    }
    for (int el : idsNear){
        if (el == myId) continue;
        Genom g = dd.getIdGen(el);
        if(compareGens(g)){
            idsNear_f.push_back(el);
        }
        idsNear_e.push_back(el);
    }
    std::vector<std::vector<int>> res = {};

    res.push_back(idsFZ_e);
    res.push_back(idsFZ_f);
    res.push_back(idsNear_e);
    res.push_back(idsNear_f);

    return res;


}

bool Organism::compareGens(Genom g){
    int diff = std::abs(_growthRate - g.growthRate) + std::abs(_maxSpeed - g.maxSpeed) + std::abs(_pointPh - g.pointPh) + std::abs(_pointPr - g.pointPh);

    if (diff < kEnemy){
        return false;
    }
    return true;

}

zone* Organism::detectZone(int xCenter, int yCenter){
    return field.getSettings(xCenter, yCenter);
}

void Organism::think(){
    Rect r = getRect();
    int xCenter = r.x() + (r.width() / 2), yCenter = r.y() + (r.width() / 2);
    zone* resZone = detectZone(xCenter, yCenter);
    std::vector<std::vector<int>> findedOrganisms = detectOrganisms(xCenter, yCenter);

    std::vector<double> input = {findedOrganisms[0].size(), findedOrganisms[1].size(), findedOrganisms[2].size(), findedOrganisms[3].size(),
     resZone->getIllumination(), resZone->getTemperature(), resZone->getViscosity()};
    std::vector<double> res = _neironNetDo->forward(input);
    double m, p, a, g;
    m = res[0];
    p = res[1];
    a = res[2];
    g = res[3];
    int countFZ = findedOrganisms[1].size() + findedOrganisms[0].size();
    int countNear = findedOrganisms[2].size() + findedOrganisms[3].size();
    if (m > p && m > a && m > g && findedOrganisms[3].size() && countNear + countFZ < kMaxNeighbour){
        multiply(findedOrganisms[3][0]);
    }
    else if (a > p && a > g && findedOrganisms[2].size()){
        attack(findedOrganisms[2][0]);
    }
    else if (g > p) {
        std::vector<int> idsFrend, idsEnemy;
        idsFrend.insert(idsFrend.end(), findedOrganisms[1].begin(), findedOrganisms[1].end());
        idsFrend.insert(idsFrend.end(), findedOrganisms[3].begin(), findedOrganisms[3].end());

        idsEnemy.insert(idsEnemy.end(), findedOrganisms[0].begin(), findedOrganisms[0].end());
        idsEnemy.insert(idsEnemy.end(), findedOrganisms[2].begin(), findedOrganisms[2].end());

        go(idsFrend, idsEnemy);
    }
    else {
        photosynthesize(resZone->getIllumination(), countNear);
    }


}
void Organism::photosynthesize(int illumination, int countNeighbors){
    _energy += (_size * _pointPh * (illumination / 10000) * kPhotosintes) / (countNeighbors + 1);
}

void Organism::multiply(int id){
    double en = kMultiply / _size;
    if (en <= _energy){
        _energy = (_energy - en);
        Genom gen = dd.getIdGen(id);

        Genom newGen;
        newGen.pointPr = std::abs(int ((_gen.pointPr + gen.pointPr) / 2 + getRandNum(-deltMutate, deltMutate, 1))) % 256;
        newGen.pointPh = 255 - newGen.pointPr;
        newGen.maxSpeed = (std::abs(int((_gen.maxSpeed + gen.maxSpeed) * 50 + getRandNum(-deltMutate, deltMutate, 1))) % 256) / 100.;
        newGen.growthRate = (std::abs(int((_gen.growthRate + gen.growthRate) * 50 + getRandNum(-deltMutate, deltMutate, 1))) % 256) / 100.;

        NeironNet NeironNetDo(*_neironNetDo);
        NeironNet NeironNetGo(*_neironNetGo);

        NeironNetDo.Mutate(deltMutate / 10., kMutate);
        NeironNetGo.Mutate(deltMutate / 10., kMutate);
        BornData data;
        data.gen = newGen;
        data.x = (getRect().x() + field.getOrganism(id)->getRect().x()) / 2;
        data.y = (getRect().y() + field.getOrganism(id)->getRect().y()) / 2;
        data.s = getRect().height() / 2;

        if (data.s < 10){
            data.s = 10;
        }
        data.gen.initial_weights_1 = NeironNetDo.printWeights();
        data.gen.initial_weights_2 = NeironNetGo.printWeights();
        std::vector<long long int> els = {getID(), _gen.convertToColor(), id, gen.convertToColor()};
        data.forBornTree = els;

        dd.addBorn(data);
        }

}

void Organism::attack(int id){
    GameObject* gb = field.getOrganism(id);

    _energy += _size * (_pointPr / 100) * kPredotor * gb->getRect().height();
    gb->interact(_size * _pointPr / 100);
}

void Organism::go(std::vector<int> idsFrend, std::vector<int> idsEnemy){
    int x_F = 0, y_F = 0, size_F = 0,  x_E= 0, y_E= 0, size_E = 0, count_F = idsFrend.size(), count_E = idsEnemy.size(), x, y;
    int delt_pos[2];
    x = getRect().x();
    y = getRect().y();
    for (int id : idsFrend){
        Rect r = field.getOrganism(id)->getRect();
        int* pos = r.position();
        int size_ = r.height();
        x_F +=  pos[0];
        y_F += pos[1];
        size_F = size_;
    }
    for (int id : idsEnemy){
        Rect r = field.getOrganism(id)->getRect();
        int* pos = r.position();
        int size_ = r.height();
        x_E += pos[0];
        y_E += pos[1];
        size_E = size_;
    }
    if (!count_F){
        count_F = 1;
    }
    if (!count_E){
        count_E = 1;
    }
    std::vector<double> input = {x_F / count_F, y_F / count_F, size_F / count_F, x_E / count_E, y_E / count_E, size_E / count_E};
    std::vector<double> output = _neironNetGo->forward(input);
    Move(output[0] * _maxSpeed, output[1] * _maxSpeed);
    inField();

    _energy -= (x + y) * _maxSpeed * kGo;

}

void Organism::growth(double grow){
//    std::cout << _energy << "\t";
    _energy = _energy - (_size * _size * kEnergyGrow);
    _size += grow * std::sqrt(_size) * kGrow;
    setSize(_size, _size);
//    std::cout << _energy << " " << _size << "\n";
    if (_size < 1 || _energy < 0){
        die();
    }
}
void Organism::die(){
    isAlive = false;
}

void Organism::inField(){

    if (getRect().x() < 0){
         getRect().setPosition((field.getWidth() + getRect().x()) % field.getWidth(), 0);
//         std::cout << 1;
    }
    if (getRect().x() > field.getWidth()){
        getRect().setPosition(getRect().x() % field.getWidth(), 0);
//        std::cout << 1;
    }
    if (getRect().y() < 0){
         getRect().setPosition(0, (field.getHeight() + getRect().y()) % field.getHeight());
//         std::cout << 1;
    }
    if (getRect().x() > field.getWidth()){
        getRect().setPosition(0, getRect().y() % field.getHeight());
//        std::cout << 1;
    }
}

Organism::~Organism()
{
    delete _neironNetDo;
    delete _neironNetGo;
}

bool Organism::IsAlive(){
    return isAlive;
}

bool Organism::operator==(const  GameObject& gameobject) const {
    return (gameobject.getID() == getID());
}
