#include "GameObject.h"
#include "rect.h"

GameObject::GameObject(Rect rect, char shape, Color col) : _rect(0, 0, 0, 0)
{
    _rect = rect;
    _shape = _shape;
    _color = col;

Rect GameObject::getRect(){
    return _rect;
}

char GameObject::getType(){
    return _shape;
}

int* GameObject::getColor(){
    return *_color;
}

GameObject::~GameObject()
{
    //dtor
}
