#ifndef NEIRON_H
#define NEIRON_H
#include<vector>

class Neiron
{
    public:
        Neiron(double b, std::vector<double> vs);
        double getRes(std::vector<double> ins);
        void changeArg(int NumArg, double arg);
        double getArg(int numArg);
        virtual ~Neiron();

    protected:

    private:
        double _b;
        std::vector<double> _vs;
};

#endif // NEIRON_H
