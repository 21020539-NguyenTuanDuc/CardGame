#include "Pointer.h"

float pointer_velo = 0.1;

void Pointer::updateTarget(int dmgGap)
{
    if(dmgGap > 5) dmgGap = 5;
    if(dmgGap <-5) dmgGap = -5;
    target.x = 237 + dmgGap * 23;
}

void Pointer::update(int deltaTime)
{
    if (pos.x < target.x)
    {
        pos.x += pointer_velo*deltaTime; // speed
    }
    else if (pos.x > target.x)
    {
        pos.x -= pointer_velo*deltaTime;
    }
    if (pos.y < target.y)
    {
        pos.y += pointer_velo*deltaTime;
    }
    else if (pos.y > target.y)
    {
        pos.y -= pointer_velo*deltaTime;
    }
    if(pos.x <= 5 + target.x && pos.x >= target.x - 5) pos.x = target.x; // fixed
    if(pos.y <= 5 + target.y && pos.y >= target.y - 5) pos.y = target.y;
}

bool Pointer::checkMove()
{
    if(pos.x <= 5 + target.x && pos.x >= target.x - 5) return true;
    else return false;
}