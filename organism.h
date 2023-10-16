#ifndef ORGANISM_H
#define ORGANISM_H
#include "Settings.h"
#include "neironnet.h"


class Organism : public GameObject
{
    public:
        Organism(double sizeo, double growthRate, double pointPh, double pointPr, double maxSpeed, Rect rect, DynamicData *dd, NeironNet* neironNet1, NeironNet* neironNet2
                 );
        virtual ~Organism();
        void update();
        void die();
        bool IsAlive();
        bool operator==(const GameObject& gameobject) const;

    protected:

    private:
        double _energy, _size, _growthRate, _pointPh, _pointPr, _maxSpeed;
        int _g1, _g2, _g3, _g4;
        long long int _gen;
        bool isAlive = true;

        NeironNet* _neironNetDo;
        NeironNet* _neironNetGo;

        void growth(double grow);
        void think();
        zone* detectZone(int xCenter, int yCenter);
        std::vector<std::vector<int>> detectOrganisms(int xCenter, int yCenter);
        bool compareGens(long long int gen);
        void multiply(int id);
        void photosynthesize(int illumination);
        void attack(int id);
        void go(std::vector<int> idsFrend, std::vector<int> idsEnemy);

};

#endif // ORGANISM_H
