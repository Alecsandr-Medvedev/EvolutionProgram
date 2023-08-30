#include <vector>

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

struct BornData{
    int gen;
    std::vector<std::vector<std::vector<double>>>* initial_weights_1;
    std::vector<std::vector<std::vector<double>>>* initial_weights_2;

};
