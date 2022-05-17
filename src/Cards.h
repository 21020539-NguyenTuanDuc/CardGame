#pragma once
#include<bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>

#include "Entity.h"

const int card_w = 90;
const int card_h = 125;

const float card_velo = 0.4;

enum SpecialAbility{
    TouchofDeath,
    Airborne,
    MightyLeap,
    None,
};

class Card
{
public:
    Vector2f pos;
    Vector2f target;
    SDL_Texture* cardTexture;
    int ATK;
    int HP;
    int Cost;
    int Ability;
    bool canMove = true;
    bool isUp = false;
public:
    Card(float _x, float _y,int _ATK, int _HP, int _Cost, int _Ability, SDL_Texture* _cardTexture){
        pos.x = _x;
        pos.y = _y;
        target = pos;
        ATK = _ATK;
        HP = _HP;
        Cost = _Cost;
        Ability = _Ability;
        cardTexture = _cardTexture;
    }
    Card()
    {
        pos;
        target;
        cardTexture = NULL;
        ATK = 0;
        HP = 0;
        Cost = 0;
        Ability = 0;
    }
    void moveUp();
    void moveDown();
    void update(int deltaTime);
    void handleEvent(SDL_Event* e);
    void handleBigScreenEvent(SDL_Event* e, Card& bigCard);
};