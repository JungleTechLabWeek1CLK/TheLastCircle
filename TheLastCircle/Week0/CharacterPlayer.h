#pragma once
#include "Character.h"

class UCharacterPlayer : public UCharacter
{
public:
    UCharacterPlayer()
    {
        Hp = 1000;
        Damage = 100;
        Delay = 1.f;
    }
    ~UCharacterPlayer()
    {
    }

private:

};