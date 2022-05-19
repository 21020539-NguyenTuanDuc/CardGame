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
    Button()
    {
        pos.x = 0;
        pos.y = 0;
        w = 0;
        h = 0;
    }
    bool handleButtonEvent(SDL_Event* e);
};