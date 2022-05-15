#pragma once
#include"Math.h"
#include"Cards.h"
#include"User.h"

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
    static void battle(Slot& Attack, Slot& Defend, User& P1, User& P2);
};