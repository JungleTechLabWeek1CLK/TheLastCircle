#pragma once
#include "Character.h"

enum class ETypeEnemy
{
    ETE_Walker,
    ETE_Runner,
    ETE_Ranger
};

class UCharacterEnemy : public UCharacter
{
public:
    ETypeEnemy EnemyType;
    float Reward = 100.f;
    UCharacterEnemy(ETypeEnemy enemyType)
    {
        Hp = 200;
        Delay = 5.f;
        bIsShoot = false;
        ShootTime = 5.f;
        EnemyType = enemyType;
        switch (EnemyType)
        {
        case ETypeEnemy::ETE_Walker:
            Range = -1.f;
            Speed = 0.1f;
            break;
        case ETypeEnemy::ETE_Runner:
            Range = -1.f;
            Speed = 0.7f;
            break;
        case ETypeEnemy::ETE_Ranger:
            Range = 0.3f;
            Speed = 0.5f;
            break;
        }
    }
    ~UCharacterEnemy()
    {
    }
    void Move(FVector location, float delta) override;
    void Attack(FVector location) override;
    void UpdateTime(float delta);
private:
};