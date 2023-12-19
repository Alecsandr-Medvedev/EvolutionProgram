#ifndef WINDOW_H
#define WINDOW_H
#include <vector>
#include <SDL.h>
#include <iostream>
#include "GameObject.h"

class window
{
    public:
        window(Color backgroundColor);
        void update();
        void draw(std::vector<GameObject*> objs);
        void startDraw();

        std::vector<Event> getEvents();
        void close();
        virtual ~window();

    protected:

    private:
        SDL_Event event;
        int _width, _height;
        std::vector<Event> events;
        SDL_Surface* screen_surface;
        SDL_Window* wind;
        SDL_Renderer* renderer;
        void drawCircle(Rect rect);
        void drawRect(Rect rect);
        void drawObject(GameObject* obj);

        void drawFillCircle(Rect rect);
        Color _backgroundColor;

};

#endif // WINDOW_H
