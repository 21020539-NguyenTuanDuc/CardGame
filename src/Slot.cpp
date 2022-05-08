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