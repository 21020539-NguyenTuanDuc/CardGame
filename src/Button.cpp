#include"Button.h"

bool Button::handleButtonEvent(SDL_Event* e)
{
    if(e->type == SDL_MOUSEBUTTONDOWN )
    {
        int x, y;
        SDL_GetMouseState( &x, &y );
        if(x >= pos.x && x <= pos.x + w && y >= pos.y && y <= pos.y + h) return true;
        else return false;
    }
    else return false;
}