#ifndef ORGANISM_H
#define ORGANISM_H
#include "GameObject.h"

#include "Settings.h"


class Organism : public GameObject
{
    public:
        Organism(int sizeo, int growthRate, int pointPh, int pointPr, int maxSpeed, Rect rect, DynamicData dd, NeironNet &neironNetDo, NeironNet &neironNetGo);
        virtual ~Organism();
        void update();
        void die();



    protected:

    private:
        int _energy, _size, _growthRate, _pointPh, _pointPr, _maxSpeed, _g1, _g2, _g3, _g4, _gen;
        bool isAlive = true;

        NeironNet _neironNetDo, _neironNetGo;

        void growth(int grow);
        void think();
        int* detectZone(int xCenter, int yCenter);
        std::vector<std::vector<int>> detectOrganisms(int xCenter, int yCenter);
        bool compareGens(int gen);
        void multiply(int gen);
        void photosynthesize(int illumination);
        void attack(int id);
        void go(std::vector<int> idsFrend, std::vector<int> idsEnemy);

};

#endif // ORGANISM_H
