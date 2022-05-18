#pragma once
#include"Math.h"
#include <SDL2/SDL.h>

class Button
{
private:
    /* data */
public:
    Vector2f pos;
    int w;
    int h;
    Button(int _x, int _y, int _w, int _h)
    {
        pos.x = _x;
        pos.y = _y;
        w = _w;
        h = _h;
    }
    bool handleButtonEvent(SDL_Event* e);
};