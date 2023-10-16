#include "window.h"
#include <string>
#include <cmath>


window::window(Color backgroundColor) : _backgroundColor(0, 0, 0)
{
    _backgroundColor = backgroundColor;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
            exit(1);
        }
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        auto width = DM.w;
        auto height = DM.h - 50;
        _width = width;
        _height = height;

        // Создание окна
        wind = SDL_CreateWindow("fsdsffsd", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (!wind) {
            SDL_Log("Unable to create window: %s", SDL_GetError());
            exit(1);
        }

        // Создание рендерера
        renderer = SDL_CreateRenderer(wind, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            SDL_Log("Unable to create renderer: %s", SDL_GetError());
            exit(1);
        }

        // Установка цвета фона
        SDL_SetRenderDrawColor(renderer, backgroundColor.r(), backgroundColor.g(), backgroundColor.b(), backgroundColor.a());

        // Очистка экрана
        SDL_RenderClear(renderer);

}

void window::update(std::vector<GameObject*> objs){
    SDL_SetRenderDrawColor(renderer, _backgroundColor.r(), _backgroundColor.g(), _backgroundColor.b(), _backgroundColor.a());
    SDL_RenderClear(renderer);

    for (GameObject* obj : objs){
        drawObject(obj);
    }
    SDL_RenderPresent(renderer);
}


void window::draw(std::vector<GameObject*> objs){}


std::vector<Event> window::getEvents(){
    events.clear();
     while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                Event eve('q');
                events.push_back(eve);
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    Event eve('q');
                    events.push_back(eve);
                }
            }
        }

    return events;
}

void window::drawRect(Rect rect){
    SDL_Rect r;
    r.x = rect.x() - (rect.width() / 2);
    r.y = rect.y() - (rect.height() / 2);
    r.w = rect.width();
    r.h = rect.height();
    SDL_RenderFillRect(renderer, &r);
}

void window::drawCircle(Rect rect){
    int offsetx, offsety, d, x, y, radius;
    int status;

     offsetx = 0;
     offsety = rect.height() / 2;
     d = (rect.height() / 2) -1;
     status = 0;
     x = rect.x();
     y = rect.y();
     radius = rect.height() / 2;


    while (offsety >= offsetx) {
             status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
             status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
             status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
             status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
             status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
             status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
             status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
             status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

            if (status < 0) {
                    status = -1;
                    break;
                }

            if (d >= 2*offsetx) {
                    d -= 2*offsetx + 1;
                    offsetx +=1;
                }
            else if (d < 2 * (radius - offsety)) {
                    d += 2 * offsety - 1;
                    offsety -= 1;
                }
            else {
                    d += 2 * (offsety - offsetx - 1);
                    offsety -= 1;
                    offsetx += 1;
                }
            }

}

void window::drawFillCircle(Rect rect){
    int offsetx, offsety, d, x, y, radius;
    int status;


    offsetx = 0;
    offsety = rect.height() / 2;
    d = (rect.height() / 2) -1;
    status = 0;
    x = rect.x();
    y = rect.y();
    radius = rect.height() / 2;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

}

void window::drawObject(GameObject *obj){
    Color c = obj->getColor();
    SDL_SetRenderDrawColor(renderer, c.r(), c.g(), c.b(), c.a());
    char type = obj->getType();
    if (type == 'r'){
        drawRect(obj->getRect());
    }
    else if (type == 'c'){
        drawCircle(obj->getRect());
    }
    else if (type == 'C'){
        drawFillCircle(obj->getRect());
    }
}



void window::close(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(wind);
    SDL_Quit();
}

window::~window()
{

}

