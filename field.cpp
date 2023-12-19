#include "field.h"

Field::Field(int width, int height, zone* baseSettings, DynamicData *dd){
    _width = width;
    _height = height;
    _baseSettings = baseSettings;
}

void Field::addOrganism(GameObject* org){
    _organismes.push_back(org);
}

void Field::addZone(zone* Zone){
    _zonesObjects.push_back(Zone);
    _zones.push_back(Zone);
}

std::vector<GameObject*> Field::getZonesObjects(){
    return _zonesObjects;
}

std::vector<int> Field::getOrganismes(Rect fieldView){
    std::vector<int> ans;
    for (GameObject* org : _organismes){
        if (fieldView.isIntersection(org->getRect())){
            ans.push_back(org->getID());
        }
    }
    return ans;
}

zone* Field::getSettings(int x, int y){
    Rect r(x, y, 1, 1);
    for (zone* z : _zones){
        if (r.isIntersection(z->getRect())){
            return z;
        }
    }
    return _baseSettings;
}

GameObject* Field::getOrganism(int id){
    for (GameObject *org : _organismes){
        if (org->getID() == id){
            return org;
        }
    }
}

int Field::getWidth()
{
 return _width;
}
int Field::getHeight(){
    return _height;
}

void Field::delOrganism(int id){
    for (int i = 0; i < _organismes.size(); i++){
        if (_organismes[i]->getID() == id){
            delete _organismes[i];
            _organismes[i] = nullptr;
        }
    }

    _organismes.erase(std::remove(_organismes.begin(), _organismes.end(), nullptr), _organismes.end());
}

Field::~Field()
{
    for (int i = 0; i < _zones.size(); i++){
        delete _zones[i];
    }
}
