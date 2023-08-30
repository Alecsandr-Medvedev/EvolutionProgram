#include "zone.h"

zone::zone(int temperature, int viscosity, int illumination, Rect rect, DynamicData dd) : GameObject(rect, 'r', Color(0, 0, 0), dd)
{
    _temperature = temperature;
    _viscosity = viscosity;
    _illumination = illumination;
}

int zone::getTemperature(){
    return _temperature;
}

int zone::getIllumination(){
    return _illumination;

}

int zone::getViscosity(){
    return _viscosity;

}

int* zone::getAll(){
    int n[3] = {_temperature, _viscosity, _illumination};
    return n;
}
zone::~zone()
{
    //dtor
}
