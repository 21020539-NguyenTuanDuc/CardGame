#include "Slot.h"


void Slot::handleEvent(SDL_Event* e, Card _sCard)
{
    //If mouse event happened
    if(e->type == SDL_MOUSEBUTTONDOWN )
    {
        //Get mouse pos
        int x, y;
        SDL_GetMouseState( &x, &y );
        if(isEmpty == true)
        {
            if(x >= pos.x && x <= pos.x + card_w && y >= pos.y && y <= pos.y + card_h)
            {
                isEmpty = false;
            }
        }
    }
}

void Slot::battle(Slot& Attack, Slot& Defend, User& P1, User& P2)
{
    if(!Attack.isEmpty)
    {
        if(Defend.isEmpty)
        {
            P2.DmgTaken -= Attack.sCard.ATK;
        }
        else
        {
            if(Attack.sCard.Ability == Airborne && Defend.sCard.Ability != MightyLeap)
            {
                P2.DmgTaken -= Attack.sCard.ATK;
            }
            else if(Attack.sCard.Ability == TouchofDeath)
            {
                Defend.sCard.HP = 0;
            }
            else
            {
                Defend.sCard.HP -= Attack.sCard.ATK;
            }
        }
    }
}