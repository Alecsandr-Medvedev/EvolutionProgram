#ifndef RECT_H
#define RECT_H
#include <cmath>

class Rect
{
    public:
        Rect(int x, int y, int width, int height);
        int x();
        int y();
        int *position();
        int width();
        int height();
        int *Size();
        void setPosition(int x, int y);
        void setSize(int w, int h);
        bool isIntersection(Rect rect, char typeIntersection='c');
        virtual ~Rect();

    protected:

    private:
    int _x, _y, _width, _height, _size[2], _position[2];
};

#endif // RECT_H
