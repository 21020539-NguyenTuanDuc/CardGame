#include "Slot.h"

void Slot::battle(Slot& Attack, Slot& Defend, User& P1, User& P2)
{
    if(!Attack.isEmpty)
    {
        if(Defend.isEmpty)
        {
            P2.DmgTaken += Attack.sCard.ATK;
        }
        else
        {
            if(Attack.sCard.Ability == Airborne && Defend.sCard.Ability != MightyLeap)
            {
                P2.DmgTaken += Attack.sCard.ATK;
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

void Slot::battleUP()
{
    sCard.target.y -= 30;
}

void Slot::battleDOWN()
{
    sCard.target.y += 30;
}

void Slot::slotUpdate()
{
    if(sCard.pos.y == sCard.target.y && sCard.pos.y != pos.y) sCard.target.y = pos.y;
}

bool Slot::checkPos()
{
    if(sCard.pos.x == sCard.target.y && sCard.pos.y == sCard.target.y) return true;
    else return false;
}