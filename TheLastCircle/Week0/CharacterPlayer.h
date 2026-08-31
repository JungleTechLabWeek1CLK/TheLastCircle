#pragma once
#include "Character.h"

class UCharacterPlayer : public UCharacter
{
public:

    UCharacterPlayer()
    {
    }
    ~UCharacterPlayer()
    {
    }
private:
    void Move();
    void Attack();
    void GetDamage();
    void Die();
};