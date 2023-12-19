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

int *Rect::position(){
    return _position;
}

bool Rect::isIntersection(Rect rect, char typeIntersection){
    int x_m, y_m, x_e, y_e;
    x_m = _x - (_width / 2);
    y_m = _y - (_height / 2);
    x_e = rect.x() - (rect.width() / 2);
    y_e = rect.y() - (rect.height() / 2);
    if (typeIntersection == 'r'){

        if (x_m + _width < x_e || x_e + rect.width() < x_m ||
            y_m + _height < y_e || y_e + rect.height() < y_m) {
            return false;
        }

        return true;
    }
    else if (typeIntersection == 'c'){
        double distance = std::sqrt(std::pow(x_m - x_e, 2) + std::pow(y_m - y_e, 2));
        return distance <= _width + rect.width();
    }
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
