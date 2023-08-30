#ifndef FIELD_H
#define FIELD_H
#include"GameObject.h"
#include<vector>
#include <map>
#include "zone.h"

class Field
{
    public:
        Field(int width, int height, zone* baseSettings, DynamicData dd);
        virtual ~Field();

        void addOrganism(GameObject org);
        void addZone(zone Zone);
        std::vector<int> getOrganismes(Rect fieldView);
        GameObject& getOrganism(int id);
        zone getSettings(int x, int y);

    protected:

    private:
        int _width, _height;
        std::vector<zone> _zones;
        zone _baseSettings;
        std::vector<GameObject> _organismes;
};

#endif // FIELD_H
