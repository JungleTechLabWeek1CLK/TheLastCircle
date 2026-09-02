#pragma once
#include "Character.h"

enum class ETypeUpgrade {
    ETU_Damage,
    ETU_Hp,
    ETU_bullets,
    ETU_Speed,
    ETU_Penetration,
    ETU_AttackSpeed,
    ETU_Scale,
    COUNT
};

class UCharacterPlayer : public UCharacter
{
public:
    int Bullets;
    int Penetration;
    float Radian; //총이 퍼지는 각도
    float EXP;
    float MaxEXP = 100;
    int Level = 1;
    bool bIsGuard = false;
    float GuardTime = 0.f;
    float GuardDelay = 1.5f;
    float MaxHp;
    float InvincibleTime = 0.f;
    float AttackSpeed = 1.5f;
    ETypeUpgrade Upgrades[7] = { ETypeUpgrade::ETU_Damage, ETypeUpgrade::ETU_Hp, ETypeUpgrade::ETU_Speed, ETypeUpgrade::ETU_bullets ,
                                        ETypeUpgrade::ETU_Penetration ,ETypeUpgrade::ETU_AttackSpeed ,ETypeUpgrade::ETU_Scale };
    FVector PlayerOffset;

    UCharacterPlayer()
    {
        MaxHp = 1000;
        Hp = 1000;
        Damage = 100;
        AttackSpeed = 1.5f;
        Speed = 0.6f;
        Bullets = 5;
        Radian = 10.f;
        Penetration = 1;
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