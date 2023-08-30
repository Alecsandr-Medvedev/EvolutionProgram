#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "MyStructs.h"
#include "rect.h"
#include "dynamicdata.h"

class GameObject
{
    public:
        GameObject(Rect rect, char shape, Color color, DynamicData dd);
        char getType();
        Rect getRect();
        Color getColor();
        void Move(int x, int y);
        int getID();
        virtual ~GameObject();
        void interact(int inter);

    private:
        Rect _rect;
        Color _color;
        char _shape;
        int _id;
        DynamicData _dd;
        int _interact;

};

#endif // GameObject_H
