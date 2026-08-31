#pragma once

#include "Physics.h"
#include "Renderer.h"


enum class ETypeCharacter
{
    ETC_Player,
    ETC_Enemy,
    ETC_PlayerProjectile,
    ETC_EnemyProjectile
};

class UCharacter
{
public:
    ////////
    // Required for test
    FVector Location;
    FVector Velocity;
    float Radius = 0.1f;
    float Mass;
    float Hp;
    float Speed;
    float Damage;
    ETypePrimitive HitBox;
    FVector Color;
    ETypeCharacter CharacterType;
    ////////

    UCharacter() : Location(0.f), Velocity(0.f)
    {
    }
    ~UCharacter()
    {
    }

    void Move();
    void Attack();
    void GetDamage();
    void Die();
private:

};
