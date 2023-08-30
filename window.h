#ifndef WINDOW_H
#define WINDOW_H
#include <vector>
#include "GameObject.h"
#include <SDL.h>
#include <iostream>

class window
{
    public:
        window(Color backgroundColor);
        void update(std::vector<GameObject> objs);


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
        void drawObject(GameObject obj);
        void draw(std::vector<GameObject> objs);
        Color _backgroundColor;

};

#endif // WINDOW_H
