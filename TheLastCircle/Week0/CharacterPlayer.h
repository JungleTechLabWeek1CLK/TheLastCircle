#pragma once
#include "Character.h"

class UCharacterPlayer : public UCharacter
{
public:
    UCharacterPlayer()
    {
        Hp = 100;
        Damage = 100;
        Delay = 1.f;
        Speed = 0.5f;
    }
    ~UCharacterPlayer()
    {
    }
    void UpdateTime(float delta);
private:

};