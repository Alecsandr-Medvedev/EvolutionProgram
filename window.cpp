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

void window::update(std::vector<GameObject> objs){

    draw(objs);
}

void window::draw(std::vector<GameObject> objs){
    SDL_SetRenderDrawColor(renderer, _backgroundColor.r(), _backgroundColor.g(), _backgroundColor.b(), _backgroundColor.a());
    SDL_RenderClear(renderer);

    for (GameObject obj : objs){
        drawObject(obj);
    }
    SDL_RenderPresent(renderer);
}

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
    r.x = rect.x();
    r.y = rect.y();
    r.w = rect.width();
    r.h = rect.height();
    SDL_RenderFillRect(renderer, &r);
}

void window::drawCircle(Rect rect){
    for (int w = 0; w < rect.width() * 2; ++w)
    {
        for (int h = 0; h < rect.height() * 2; ++h)
        {
            int dx = rect.width() - w;
            int dy = rect.height() - h;

            if (sqrt(dx * dx + dy * dy) <= rect.height() / 2)
            {
                SDL_RenderDrawPoint(renderer, rect.x() + dx + rect.width() / 2, rect.y() + dy + rect.height() / 2);
            }
        }
    }
}

void window::drawObject(GameObject obj){

    Color c = obj.getColor();
    SDL_SetRenderDrawColor(renderer, c.r(), c.g(), c.b(), c.a());
    char type = obj.getType();
    if (type == 'r'){
        drawRect(obj.getRect());
    }
    else if (type == 'c'){
        drawCircle(obj.getRect());
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

