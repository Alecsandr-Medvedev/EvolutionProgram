#include "rect.h"

Rect::Rect(int x, int y, int width, int height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    _position[0] = x;
    _position[1] = y;
    _size[0] = width;
    _size[1] = height;

}

Rect::x(){
    return _x;
}

Rect::y(){
    return _y;
}

Rect::width(){
    return _width;
}

Rect::height(){
    return _height;
}

int *Rect::Size(){
    return _size;
}

int *Rect::posittion(){
    return _position;
}

bool Rect::isIntersection(Rect rect){
    if (_x + _width < rect.x() || rect.x() + rect.width() < _x ||
        _y + _height < rect.y() || rect.y() + rect.height() < _y) {
        return false;
    }

    return true;
}

void Rect::setPosition(int x, int y){
    _x = x;
    _y = y;
}

void Rect::setSize(int w, int h){
    if (w > 0 && h > 0){
        _width = w;
        _height = h;
    }

}

Rect::~Rect()
{
    //dtor
}
