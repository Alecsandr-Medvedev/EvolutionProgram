#include "organism.h"

Organism::Organism(double sizeo, double growthRate, double pointPh, double pointPr, double maxSpeed, Rect rect, DynamicData *dd, NeironNet* neironNet1, NeironNet* neironNet2) : GameObject(rect, 'c', Color(int(2.56 * pointPr), int(2.56 * pointPh), 256 - maxSpeed, 256 - growthRate), dd), _neironNetDo(neironNet1), _neironNetGo(neironNet2){
    _energy = sizeo * sizeo;
    _size = sizeo;
    _growthRate = growthRate;
    _pointPh = pointPh;
    _pointPr = pointPr;
    _maxSpeed = maxSpeed;

    _g1 = int(2.56 * pointPr);
    _g2 = int(2.56 * pointPh);
    _g3 = (256 - maxSpeed);
    _g4 = (256 - (growthRate * 100));

    _gen = _g1 * 0x1000000 +_g2 * 0x10000 + _g3 * 0x100 + _g4;
    _neironNetDo = neironNet1;
    _neironNetGo = neironNet2;



}

void Organism::update(){
    _size -= _interact;
    _interact = 0;
    growth(_growthRate);
	think();

}

std::vector<std::vector<int>> Organism::detectOrganisms(int xCenter, int yCenter){
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
        if (el == getID()) continue;
        int g = field.getOrganism(el)->getColor().rgba();
        if(compareGens(g)){
            idsFZ_f.push_back(el);
        }
        idsFZ_e.push_back(el);
    }
    for (int el : idsNear){
        if (el == getID()) continue;
        int g = field.getOrganism(el)->getColor().rgba();
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

bool Organism::compareGens(long long int gen){
    int g1, g2, g3, g4;
    g1 = gen / 0x1000000;
    g2 = (gen - g1) / 0x10000;
    g3 = (gen - g1 - g2) / 0x100;
    g4 = gen % 0x100;

    int diff = std::abs(_g1 - g1) + std::abs(_g2 - g2) + std::abs(_g3 - g3) + std::abs(_g4 - g4);

    if (diff < kEnemy ){
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
    double res = _neironNetDo->forward(input)[0];
    if (res <= 0.25 && findedOrganisms[3].size()){
        multiply(findedOrganisms[3][0]);
    }
    else if (res <= 0.5 && res > 0.25){
        photosynthesize(resZone->getIllumination());
    }
    else if (res <= 0.75 && res > 0.5 && findedOrganisms[2].size()){
        attack(findedOrganisms[2][0]);
    }
    else if (res > 0.75) {
        std::vector<int> idsFrend, idsEnemy;
        idsFrend.insert(idsFrend.end(), findedOrganisms[1].begin(), findedOrganisms[1].end());
        idsFrend.insert(idsFrend.end(), findedOrganisms[3].begin(), findedOrganisms[3].end());

        idsEnemy.insert(idsEnemy.end(), findedOrganisms[0].begin(), findedOrganisms[0].end());
        idsEnemy.insert(idsEnemy.end(), findedOrganisms[2].begin(), findedOrganisms[2].end());

        go(idsFrend, idsEnemy);
    }

}
void Organism::photosynthesize(int illumination){
    _energy += _size * _pointPh * illumination / 10000 * kPhotosintes;
}

void Organism::multiply(int id){
    double en = kMultiply / _size;
    if (en <= _energy){
        _energy = (_energy - en);
        long long int gen = dd.getIdGen(id);
        int g1, g2, g3, g4;
        g1 = gen / 0x1000000;
        g2 = (gen - g1) / 0x10000;
        g3 = (gen - g1 - g2) / 0x100;
        g4 = gen % 0x100;
    //    std::cout << "-" << _g1 << " " << _g2 << " " << _g3 << " " << _g4 << " " << _gen << "-";
        long long int newGen = abs((_g1 + g1) / 2 + getRandNum(-deltMutate, deltMutate, 0.1)) * 0x1000000 +
                     abs((_g2 + g2) / 2 + getRandNum(-deltMutate, deltMutate, 0.1)) * 0x10000 +
                     abs((_g3 + g3) / 2 + getRandNum(-deltMutate, deltMutate, 0.1)) * 0x100 +
                     abs((_g4 + g4) * 50 + getRandNum(-deltMutate, deltMutate, 0.1));

        NeironNet NeironNetDo(*_neironNetDo);
        NeironNet NeironNetGo(*_neironNetGo);

        NeironNetDo.Mutate(deltMutate, kMutate);
        NeironNetGo.Mutate(deltMutate, kMutate);
        BornData data;
        data.gen = newGen;
        data.x = (getRect().x() + field.getOrganism(id)->getRect().x()) / 2;
        data.y = (getRect().y() + field.getOrganism(id)->getRect().y()) / 2;
        data.s = getRect().height() / 2;

        if (data.s <= 10){
            data.s = 10;
        }
        data.initial_weights_1 = NeironNetDo.printWeights();
        data.initial_weights_2 = NeironNetGo.printWeights();
        std::vector<long long int> els = {getID(), id, _gen, gen};
        data.forBornTree = els;

        dd.addBorn(data);
        }

}

void Organism::attack(int id){
    GameObject* gb = field.getOrganism(id);

    _energy += _size * _pointPr / 100 * kPredotor * gb->getRect().height();
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
    Move((field.getWidth() + getRect().x()) % field.getWidth(), (field.getHeight() + getRect().y()) % field.getHeight());
    _energy -= (x + y) * _maxSpeed * kGo;

}

void Organism::growth(double grow){
//    std::cout << grow << "\t";
    _energy = _energy - (_size * _size * kEnergyGrow);
    _size += grow;
    setSize(_size, _size);
    if (_size < 1 || _energy < 0){
        die();
    }
}
void Organism::die(){
    isAlive = false;
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
