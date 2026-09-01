#pragma once
#include "CharacterEnemy.h"

class UCharacterEnemyRanger : public UCharacterEnemy
{
public:
    float Delay = 0.1f;
    float ShootTime = 0;
    bool bIsShoot = true;

    /*UCharacterEnemyRanger()
    {
        Hp = 1000;
        Damage = 100;
    }
    ~UCharacterEnemyRanger()
    {
    }*/
    void UpdateTime(float delta);
    void Move(FVector location, float delta) override;
private:

};