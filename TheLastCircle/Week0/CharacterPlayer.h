#pragma once
#include "Character.h"

class UCharacterPlayer : public UCharacter
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

    UCharacterPlayer() : Location(0.f), Velocity(0.f)
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