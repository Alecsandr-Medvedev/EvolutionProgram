#include "GameObject.h"
#include "rect.h"

GameObject::GameObject(Rect rect, char shape, Color color, DynamicData *dd) : _color(139, 0, 255), _rect(0, 0, 0, 0)
{
    _rect = rect;
    _shape = shape;
    _color = color;
    _dd = dd;
    _id = _dd->getFreeId();


}

Rect GameObject::getRect(){
    return _rect;
}

char GameObject::getType(){
    return _shape;
}

Color GameObject::getColor(){
 return _color;
}

int GameObject::getID() const{
 return _id;
}

GameObject::~GameObject()
{

    _dd->addFreeId(_id);
}

void GameObject::Move(int x, int y){
    _rect.setPosition(_rect.x() + x, _rect.y() + y);
}

void GameObject::interact(int inter){
    _interact = inter;
}

void GameObject::setSize(int width, int height){
    _rect.setSize(width, height);

}



