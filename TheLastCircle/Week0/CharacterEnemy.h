#pragma once
#include "Character.h"

class UCharacterEnemy : public UCharacter
{
public:
    
    UCharacterEnemy()
    {
    }
    ~UCharacterEnemy()
    {
    }
private:
    void Move();
    void Attack();
    void GetDamage();
    void Die();
};