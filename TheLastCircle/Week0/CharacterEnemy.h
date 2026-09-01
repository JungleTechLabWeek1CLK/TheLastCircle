#pragma once
#include "Character.h"

class UCharacterEnemy : public UCharacter
{
public:
    
    UCharacterEnemy()
    {
        Hp = 100;
        Range = 0.3f;
        Delay = 5.f;
    }
    ~UCharacterEnemy()
    {
    }
    void Move(FVector location, float delta) override;
    void Attack(FVector location) override;
private:
};