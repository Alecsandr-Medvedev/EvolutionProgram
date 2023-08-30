#include "color.h"

Color::Color(int r, int g, int b, int a)
    : _r(r), _g(g), _b(b), _a(a)
{
}

void Color::ChangeColor(int r, int g, int b, int a)
{

}

int* Color::GetColor()
{

}

int Color::r() const
{
    return _r;
}

int Color::g() const
{
    return _g;
}

int Color::b() const
{
    return _b;
}

int Color::a() const
{
    return _a;
}

Color::~Color()
{
    // Пустой деструктор
}
