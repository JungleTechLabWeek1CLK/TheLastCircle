#pragma once
#include "Character.h"

class UCharacterEnemy : public UCharacter
{
public:
    ////////
    // Required for test
    FVector Location;
    FVector Velocity;
    float Radius;
    float Mass;
    float Hp;
    float Speed;
    float Damage;
    ETypePrimitive HitBox;
    FVector Color;
    ////////

    UCharacterEnemy() : Location(0.f), Velocity(0.f)
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