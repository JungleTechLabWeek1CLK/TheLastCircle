#pragma once
#include "Character.h"

enum class ETypeUpgrade {
    ETU_Damage,
    ETU_Hp,
    ETU_bullets,
    ETU_Speed
};

class UCharacterPlayer : public UCharacter
{
public:
    int Bullets;
    float Radian;
    float EXP;
    float MaxEXP = 100;
    int Level = 1;
    bool bIsGuard = false;
    float GuardTime = 0.f;
    float GuardDelay = 3.f;
    UCharacterPlayer()
    {
        Hp = 100;
        Damage = 100;
        Delay = 1.f;
        Speed = 0.5f;
        Bullets = 5;
        Radian = 10.f;
    }
    ~UCharacterPlayer()
    {
    }
    void GetDamage(float damage) override;
    void UpdateTime(float delta);
    void LevelUp();
    void Upgrade();
    void GetEXP(int exp);
private:

};