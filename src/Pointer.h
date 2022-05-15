#pragma once

#include <SDL2/SDL.h>

#include"Math.h"
class Pointer
{
public:
    Vector2f pos;
    Vector2f target;
    SDL_Texture* pointerTex;
    Pointer(float _x , float _y, SDL_Texture* _pointerTex)
    {
        pos.x = _x;
        pos.y = _y;
        target = pos;
        pointerTex = _pointerTex;
    }
    void updateTarget(int dmgGap);
    void update(int deltaTime);
    bool checkMove();
};
