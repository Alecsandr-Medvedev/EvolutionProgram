#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "rect.h"
#include "color.h"

class GameObject
{
public:
    GameObject(Rect rect, char shape, Color col);
    char getType();
    Rect getRect();
    int *getColor();
    virtual ~GameObject();

private:
    Rect _rect;
    char _shape;
    Color _color;
};

#endif // GameObject_H
