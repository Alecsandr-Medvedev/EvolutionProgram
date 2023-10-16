#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#pragma once
#include "rect.h"
#include "dynamicdata.h"
#include <iostream>

class GameObject
{
    public:
        GameObject(Rect rect, char shape, Color color, DynamicData *dd);
        char getType();
        Rect getRect();
        Color getColor();
        void Move(int x, int y);
        int getID() const;
        virtual ~GameObject();
        void interact(int inter);
        void setSize(int width, int height);

    protected:
        int _interact = 0;


    private:
        Rect _rect;
        Color _color;
        char _shape;
        int _id;
        DynamicData *_dd;

};

#endif // GameObject_H
