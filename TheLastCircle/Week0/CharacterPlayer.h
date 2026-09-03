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
    ETU_Axe,
    ETU_Garlic,
    ETU_Bible,
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
    bool bIsAxe = false;
    float GarlicDamage = 0.1f;
    int AxeCnt = 0;
    float AxeDelay = 2.f;
    float AxeTime = 0.f;
    float GuardTime = 0.f;
    float GuardDelay = 1.5f;
    float MaxHp;
    float InvincibleTime = 0.f;
    float AttackSpeed = 1.5f;

    int MaxBible = 0;
    int BibleCnt = 0;

    ETypeUpgrade Upgrades[3] = { ETypeUpgrade::ETU_Axe, ETypeUpgrade::ETU_Garlic, ETypeUpgrade::ETU_Bible };
    ETypeUpgrade SpecialUpgrade[5] = { ETypeUpgrade::ETU_bullets ,ETypeUpgrade::ETU_Penetration,ETypeUpgrade::ETU_Axe, ETypeUpgrade::ETU_Garlic, ETypeUpgrade::ETU_Bible };
    ETypeUpgrade NormalUpgrade[5] = { ETypeUpgrade::ETU_Damage, ETypeUpgrade::ETU_Hp, ETypeUpgrade::ETU_Speed,ETypeUpgrade::ETU_AttackSpeed ,ETypeUpgrade::ETU_Scale };

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
    void UpBible(){}
private:

};