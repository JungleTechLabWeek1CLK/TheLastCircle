#pragma once
#include "Character.h"

class UCharacterPlayer : public UCharacter
{
public:
    float Delay = 1;
    float ShootTime = 0;
    bool bIsShoot = true;

    UCharacterPlayer()
    {
    }
    ~UCharacterPlayer()
    {
    }
    void UpdateTime(float delta);

private:

};