#pragma once
#include "Character.h"

enum class ETypeUpgrade {
    ETU_Damage,
    ETU_Hp,
    ETU_bullets,
    ETU_Speed,
    COUNT
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
    float InvincibleTime = 0.f;
    ETypeUpgrade Upgrades[4] = {ETypeUpgrade::ETU_Damage, ETypeUpgrade::ETU_Hp, ETypeUpgrade::ETU_Speed, ETypeUpgrade::ETU_bullets };

    UCharacterPlayer()
    {
        MaxHp = 1000;
        Hp = 1000;
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
    ETypeUpgrade* RandomUpgrade();
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