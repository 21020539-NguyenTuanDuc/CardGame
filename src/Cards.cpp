#include "Cards.h"
#include "User.h"

#include<iostream>

Mix_Chunk* clickSFX = Mix_LoadWAV("res/sfx/click.mp3");

void Card::moveUp()
{
    if(isUp == false)
    {
        target.x = pos.x;
        target.y = pos.y - 30; // up distance
        isUp = true;
    }
}

void Card::moveDown()
{
    if(isUp == true)
    {
        target.x = pos.x;
        target.y = pos.y + 30; // down distance
        isUp = false;
    }
}

void Card::update(int deltaTime)
{
    if (pos.x < target.x)
    {
        pos.x += card_velo*deltaTime; // speed
    }
    else if (pos.x > target.x)
    {
        pos.x -= card_velo*deltaTime;
    }
    if (pos.y < target.y)
    {
        pos.y += card_velo*deltaTime;
    }
    else if (pos.y > target.y)
    {
        pos.y -= card_velo*deltaTime;
    }
    if(pos.x <= 5 + target.x && pos.x >= target.x - 5) pos.x = target.x; // fixed
    if(pos.y <= 5 + target.y && pos.y >= target.y - 5) pos.y = target.y;
}

bool Card::handleEvent(SDL_Event* e)
{
    //If mouse event happened
    if(e->type == SDL_MOUSEBUTTONDOWN )
    {
        //Get mouse pos
        int x, y;
        SDL_GetMouseState( &x, &y );
        if(x >= pos.x && x <= pos.x + card_w && y >= pos.y && y <= pos.y + card_h)
        {
            if(isUp == false)
            {               
                moveUp();
                return true;
            }
        }
        else
        {
            moveDown();
            return false;
        }
    }
    return false;
}

void Card::handleBigScreenEvent(SDL_Event*e, Card& bigCard)
{
    if(e->type == SDL_MOUSEBUTTONDOWN)
    {
        //Get mouse pos
        int x, y;
        SDL_GetMouseState( &x, &y );
        if(x >= pos.x && x <= pos.x + card_w && y >= pos.y && y <= pos.y + card_h)
        {
            bigCard.cardTexture = cardTexture;
        }
    }
}