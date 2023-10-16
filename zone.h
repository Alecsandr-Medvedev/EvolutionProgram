#ifndef ZONE_H
#define ZONE_H
#include "GameObject.h"

class zone : public GameObject
{
    public:
        zone(int temperature, int viscosity, int illumination, Rect rect, DynamicData *dd);
        virtual ~zone();
        int getTemperature();
        int getViscosity();
        int getIllumination();
        int* getAll();

    protected:

    private:
        int _temperature, _viscosity, _illumination;
};

#endif // ZONE_H
