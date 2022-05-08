#pragma once
#include<vector>
#include<random>
#include<algorithm>

#include"Cards.h"
#include "RenderWindow.h"

const int startPos_x = 1255;
const int startPos_y = 535;

SDL_Texture* Adder = window.loadTexture("res/gfx/Adder.jpg");
SDL_Texture* Bee = window.loadTexture("res/gfx/Bee.jpg");
// SDL_Texture* Bee1 = window.loadTexture("res/gfx/Bee.jpg");
SDL_Texture* Bullfrog = window.loadTexture("res/gfx/Bullfrog.jpg");
SDL_Texture* Coyote = window.loadTexture("res/gfx/Coyote.jpg");
SDL_Texture* Geck = window.loadTexture("res/gfx/Geck.jpg");
// SDL_Texture* Geck1 = window.loadTexture("res/gfx/Geck.jpg");
SDL_Texture* Grizzly = window.loadTexture("res/gfx/Grizzly.jpg");
SDL_Texture* Hawk = window.loadTexture("res/gfx/Hawk.jpg");
SDL_Texture* Rabbit = window.loadTexture("res/gfx/Rabbit.jpg");
// SDL_Texture* Rabbit1 = window.loadTexture("res/gfx/Adder.jpg");
SDL_Texture* Raven = window.loadTexture("res/gfx/Raven.jpg");
SDL_Texture* RiverSnapper = window.loadTexture("res/gfx/RiverSnapper.jpg");
SDL_Texture* Squirrel = window.loadTexture("res/gfx/Squirrel.jpg");
SDL_Texture* Stoat = window.loadTexture("res/gfx/Stoat.jpg");
SDL_Texture* Undeadcat = window.loadTexture("res/gfx/Undeadcat.jpg");
SDL_Texture* Urayuli = window.loadTexture("res/gfx/Urayuli.jpg");
SDL_Texture* Wolf = window.loadTexture("res/gfx/Wolf.jpg");

std::vector<Card> Deck = {
    Card(startPos_x, startPos_y, 1, 1, 2, TouchofDeath, Adder),
    Card(startPos_x, startPos_y, 1, 1, 0, None, Bee),
    Card(startPos_x, startPos_y, 1, 1, 0, None, Bee),
    Card(startPos_x, startPos_y, 1, 2, 1, MightyLeap, Bullfrog),
    Card(startPos_x, startPos_y, 2, 1, 0, None, Coyote),
    Card(startPos_x, startPos_y, 1, 1, 0, None, Geck),
    Card(startPos_x, startPos_y, 1, 1, 0, None, Geck),
    Card(startPos_x, startPos_y, 4, 6, 3, None, Grizzly),
    Card(startPos_x, startPos_y, 3, 1, 2, Airborne, Hawk),
    Card(startPos_x, startPos_y, 0, 1, 0, None, Rabbit),
    Card(startPos_x, startPos_y, 0, 1, 0, None, Rabbit),
    Card(startPos_x, startPos_y, 2, 3, 2, Airborne, Raven),
    Card(startPos_x, startPos_y, 1, 6, 2, None, RiverSnapper),
    Card(startPos_x, startPos_y, 0, 1, 0, None, Squirrel),
    Card(startPos_x, startPos_y, 0, 1, 0, None, Squirrel),
    Card(startPos_x, startPos_y, 1, 2, 1, None, Stoat),
    Card(startPos_x, startPos_y, 3, 6, 1, None, Undeadcat),
    Card(startPos_x, startPos_y, 7, 7, 4, None, Urayuli),
    Card(startPos_x, startPos_y, 3, 2, 2, None, Wolf)
};

std::vector<Card> Hand;

void getCard()
{
	for(int i = 0;i < 5;i++) // number of start card
	{
		Hand.push_back(Deck[0]);
		Deck.erase(Deck.begin());
	}
}

void updateHand()
{
    int sz = Hand.size();
    if(sz%2==1)
    {
        for(int i=0;i<sz;i++)
        {
            Hand[i].target.x = Width/2 - card_w/2 - 127 * (sz/2 - i); // 127 = card distance
        }
    }
    else
    {
        for(int i=0;i<sz/2;i++)
        {
            Hand[i].target.x = Width/2 - (127-card_w)/2 - card_w - 127*(sz/2 - 1 - i);
        }
        for(int i=sz/2;i<sz;i++)
        {
            Hand[i].target.x = Width/2 + (127-card_w)/2 + 127*(i-sz/2);
        }
    }
}

void drawCard()
{   
    if(Deck.size()!=0)
    {
        Hand.push_back(Deck[0]);
	    Deck.erase(Deck.begin());
    }
}

void shuffleDeck()
{
    unsigned randSeed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(std::begin(Deck), std::end(Deck), std::default_random_engine(randSeed));
}