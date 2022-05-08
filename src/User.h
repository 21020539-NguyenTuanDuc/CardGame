#pragma once
#include<bits/stdc++.h>

class User{
public:
    int DmgTaken;
    User(int _DmgTaken = 0){
        DmgTaken = _DmgTaken;
    }
};

User Player;
User Bot;