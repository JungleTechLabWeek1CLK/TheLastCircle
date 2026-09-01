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
    float MaxHp;
    ETypeUpgrade Upgrades[3] = {ETypeUpgrade::ETU_Damage, ETypeUpgrade::ETU_Hp, ETypeUpgrade::ETU_Speed};

    UCharacterPlayer()
    {
        MaxHp = 100;
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
    int LevelUp();
    void Upgrade(ETypeUpgrade type);
    void GetEXP(int exp);
    int GetLevel() {
        return Level;
    }
    float GetCurrentEXP() {
        return EXP;
    }
    float GetMaxEXP() {
        return MaxEXP;
    }
    float GetCurrentHp() {
        return Hp;
    }
    float GetMaxHp() {
        return MaxHp;
    }
private:

};