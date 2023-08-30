#include "neiron.h"

Neiron::Neiron(double b, std::vector<double> vs)
{
    _b = b;
    _vs = vs;
}

double Neiron::getRes(std::vector<double> ins){
    double ans = _b;
    for (int i = 0; i < ins.size(); i++){
        ans += ins[i] * _vs[i];
    }
}
void Neiron::changeArg(int NumArg, double arg){
    if (NumArg == -1){
        _b = arg;
    }
    else{
        _vs[NumArg] = arg;
    }
}
double Neiron::getArg(int numArg){
    if (numArg == -1){
        return _b;
    }
    else{
        return _vs[numArg];
    }
}

Neiron::~Neiron()
{
    //dtor
}
