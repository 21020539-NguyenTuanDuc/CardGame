#pragma once
#include "RenderWindow.h"

class cursor
{
public:
    SDL_Texture* t_cursor;
    SDL_Rect r_cursor;
    SDL_Rect p_cursor; 

    Mouse()
    {
        t_cursor = window.loadTexture("res/gfx/Urayuli.png");
        r_cursor.w = 25;
        r_cursor.h = 25;
        p_cursor.w = 1;
        p_cursor.h = 1;

        SDL_ShowCursor(false);
    }
    void update()
    {
        SDL_GetMouseState(&r_cursor.x, &r_cursor.y);
        p_cursor.x = r_cursor.x;
        p_cursor.y = r_cursor.y;
    }
    void draw()
    {
        SDL_RenderCopy(window.renderer, t_cursor, NULL, &r_cursor);
    }
};