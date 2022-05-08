#pragma once
#include"Math.h"
#include"Cards.h"

class Slot
{
public:
    Vector2f pos;
    Card sCard;
    int w = card_w;
    int h = card_h;
    bool isEmpty = true;
    bool isChoosing = false;
    Slot(int _x, int _y)
    {
        pos.x = _x;
        pos.y = _y;
    }
    void handleEvent(SDL_Event* e, Card _sCard);
};

// Slot S1(400, 328);
// Slot S2(515, 328);
// Slot S3(630, 328);
// Slot S4(744, 328);