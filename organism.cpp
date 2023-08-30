#include "organism.h"

Organism::Organism(int sizeo, int growthRate, int pointPh, int pointPr, int maxSpeed, Rect rect, DynamicData dd, NeironNet &neironNetDo, NeironNet &neironNetGo) : GameObject(rect, 'c', Color(int(2.56 * pointPr), int(2.56 * pointPh), 256 - maxSpeed, 256 - growthRate), dd), _neironNetDo(StandartNeiron1), _neironNetGo(StandartNeiron2, NeironNet::ActivationFunctionType::Tanh)
{
    _energy = sizeo * sizeo;
    _size = sizeo;
    _growthRate = growthRate;
    _pointPh = pointPh;
    _pointPr = pointPr;
    _maxSpeed = maxSpeed;
    _neironNetDo = neironNetDo;
    _neironNetGo = neironNetGo;

    _g1 = int(2.56 * pointPr);
    _g2 = int(2.56 * pointPh);
    _g3 = (256 - maxSpeed);
    _g4 = (256 - growthRate);

    _gen = _g1 * 0x1000000 +_g2 * 0x10000 + _g3 * 0x100 + _g4;

}

void Organism::update(){
    _size -= _interact;
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

    std::vector<int> idsFZ_e, idsNear_e, idsFZ_f, idsNear_f;
    for (int el : idsFZ){
        if(compareGens(dd.getIdGen(el))){
            idsFZ_f.push_back(el);
        }
        idsFZ_e.push_back(el);
    }
    for (int el : idsNear){
        if(compareGens(dd.getIdGen(el))){
            idsNear_f.push_back(el);
        }
        idsNear_e.push_back(el);
    }
    std::vector<std::vector<int>> res = {idsFZ_e, idsFZ_f, idsNear_e, idsNear_f};
    return res;

}

bool Organism::compareGens(int gen){
    int g1, g2, g3, g4;
    g1 = gen / 0x1000000;
    g2 = (gen - g1) / 0x10000;
    g3 = (gen - g1 - g2) / 0x100;
    g4 = gen % 0x100;
    int diff = std::abs(_g1 - g1) + std::abs(_g2 - g2) + std::abs(_g3 - g3) + std::abs(_g4 - g4);
    if (diff > kEnemy){
        return false;
    }
    return true;

}

int* Organism::detectZone(int xCenter, int yCenter){
    return field.getSettings(xCenter, yCenter).getAll();
}

void Organism::think(){
    Rect r = getRect();
    int xCenter = r.x() + (r.width() / 2), yCenter = r.y() + (r.width() / 2);
    int* resZone = detectZone(xCenter, yCenter);
    std::vector<std::vector<int>> findedOrganisms = detectOrganisms(xCenter, yCenter);
    std::vector<double> input = {findedOrganisms[0].size(), findedOrganisms[1].size(), findedOrganisms[2].size(), findedOrganisms[3].size(), resZone[0],
    resZone[1], resZone[2]};
    double res = _neironNetDo.forward(input)[0];
    if (res <= 0.25 && findedOrganisms[3].size()){
        multiply(dd.getIdGen(findedOrganisms[3][0]));
    }
    else if (res <= 0.5){
        photosynthesize(resZone[2]);
    }
    else if (res <= 0.75 && findedOrganisms[3].size()){
        attack(findedOrganisms[2][0]);
    }
    else {
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

void Organism::multiply(int gen){
    int g1, g2, g3, g4;
    g1 = gen / 0x1000000;
    g2 = (gen - g1) / 0x10000;
    g3 = (gen - g1 - g2) / 0x100;
    g4 = gen % 0x100;
    int newGen = ((_g1 + g1) / 2 + getRandNum(-kMutate, kMutate, 0.1)) +
    ((_g2 + g2) / 2 + getRandNum(-kMutate, kMutate, 0.1)) +
    ((_g3 + g3) / 2 + getRandNum(-kMutate, kMutate, 0.1)) +
    ((_g4 + g4) / 2 + getRandNum(-kMutate, kMutate, 0.1));

    NeironNet NeironNetDo(_neironNetDo);
    NeironNet NeironNetGo(_neironNetGo);

    NeironNetDo.Mutate(kMutate / 10.0, 0.1);
    NeironNetGo.Mutate(kMutate / 10.0, 0.1);
    BornData data;
    data.gen = newGen;
    data.initial_weights_1 = *NeironNetDo.printWeights();
    data.initial_weights_2 = *NeironNetGo.printWeights();

    dd.addBorn(data);
}

void attack(int id){
    GameObject& gb = field.getOrganism(id);
    _energy += _size * _pointPr / 100 * kPredotor * gb.getRect().height();
    gb.interact(_size * _pointPr / 100);
}

void go(std::vector<int> idsFrend, std::vector<int> idsEnemy){
    std::vector<double> output;
    int x = 0, y = 0;
    for (int id : idsFrend){
        int pos[2] = field.getOrganism(id).getRect().posittion();
        std::vector<double> input =  {pos[0], pos[1], 1};
        output = _neironNetGo(input);
        x += output[0];
        y += input[1];
    }
    for (int id : idsEnemy){
        int pos[2] = field.getOrganism(id).getRect().posittion();
        std::vector<double> input =  {pos[0], pos[1], -1};
        output = _neironNetGo(input);
        x += output[0];
        y += input[1];
    }
    Move(x * _maxSpeed, y * _maxSpeed);
    _energy -= (x + y) * _maxSpeed;

}

void Organism::growth(int grow){
    _energy -= _size * _size * kEnergyGrow;
    _size += grow;
    getRect().setSize(_size, _size);

    if (_size < 1 || _energy < 0){
        die();
    }
}
void Organism::die(){
    isAlive = false;
}
Organism::~Organism()
{
    //dtor
}

bool Organism::IsAlive(){
    return isAlive;
}

