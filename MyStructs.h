#include <vector>
#include <iostream>

struct Color{
    public:
        Color(int r, int g, int b, int a = 255) : _r(r), _g(g), _b(b), _a(a){}
        void ChangeColor(int r, int g, int b, int a = 255) {
            _r = r;
            _g = g;
            _b = b;
            _a = a;
        }
        int* GetColor(){
            static int color[4] = {_r, _g, _b, _a};
            return color;
        }
        int r() const {
            return _r;
        }
        int g() const {
            return _g;
        }
        int b() const {
            return _b;
        }
        int a() const {
            return _a;
        }
        int rgba(){
//            std::cout << (_r * 0x1000000) + (_g * 0x10000) + (_b * 0x100) + _a << "-";
            return (_r * 0x1000000) + (_g * 0x10000) + (_b * 0x100) + _a;
        }
    private:
        int _r, _g, _b,_a;

};

struct Event{
    public:
        Event(char type, int x = 0, int y = 0, char key = '_'){
            _position[0] = x;
            _position[1] = y;
            _type = type;
            _key = key;
        }
        int *getPosition(){
            return _position;
        }
        char getType(){
            return _type;
        }
        char getKey(){
            return _key;
            }


    private:
        char _type;
        int _position[2], _key;

};

struct Genom{
    double maxSpeed, growthRate, pointPr, pointPh;
    std::vector<std::vector<std::vector<double>>> initial_weights_1, initial_weights_2;

    long long int convertToColor(){
        long long int g1 = pointPr;
        g1 *= 0x1000000;
        long long int g2 = pointPh * 0x10000;
        long long int g3 = (256 - (maxSpeed * 100)) * 0x100;
        long long int g4 = (256 - (growthRate * 100));
//        std::cout << g1 << " " << g2 << " " << g3 << " " << g4 << "\n";
        return g1 + g2 + g3 + g4;
    };
};

struct BornData{
    int x, y, s;
    Genom gen;
    std::vector<long long int> forBornTree;

};


