#include "Physics.h"
#include <Windows.h>

#include "Character.h"
#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"



/*
필수적인 구현 내용

GetDamage -> 연속적으로 call이 되도, 자체적으로 일정 시간 후에만 처리되도록 해야함
Die -> projectile의 die가 예상한대로 잘 동작해야함
*/
// TODO: 인자 개수 줄이기
void HandleCollision(UCharacterPlayer* Player, UCharacterEnemy** EnemyList, INT32 EnemyListCount,
    UProjectile** ProjectilePlayerList, INT32 ProjectilePlayerListCount, UProjectile** ProjectileEnemyList, INT32 ProjectileEnemyListCount, const float DELTA_TIME)
{
    // ----------
    // detecting collision regarding player
    // Player - Enemy 
    for (INT32 CurrentIndex = 0; CurrentIndex < EnemyListCount; ++CurrentIndex)
    {
        UCharacterEnemy* CurrentEnemy = EnemyList[CurrentIndex];

        // Sphere - Sphere Collision
        FVector CollisionNormal = CurrentEnemy->Location - Player->Location; // not normalized yet
        const float DISTANCE = CollisionNormal.GetMagnitude();

        if (DISTANCE < (Player->Radius + CurrentEnemy->Radius))
        {
            // collision detected
            Player->GetDamage();
        }
    }
    // Player - Enemy Projectile 
    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileEnemyListCount; ++CurrentIndex)
    {
        UProjectile* CurrentEnemyProjectile = ProjectileEnemyList[CurrentIndex];

        // Sphere - Sphere Collision
        FVector CollisionNormal = CurrentEnemyProjectile->Location - Player->Location; // not normalized yet
        const float DISTANCE = CollisionNormal.GetMagnitude();

        if (DISTANCE < (Player->Radius + CurrentEnemyProjectile->Radius))
        {
            // collision detected
            Player->GetDamage();

            CurrentEnemyProjectile->Die();
        }
    }
    // ----------



    // detecting collision regarding enemy
    // Player Projectile - Enemy
    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectilePlayerListCount; ++CurrentIndex)
    {
        UCharacter* CurrentCharacterProjectile = ProjectileEnemyList[CurrentIndex];
        
        for (INT32 CurrentNestedIndex = 0; CurrentNestedIndex < EnemyListCount; ++CurrentNestedIndex)
        {
            UCharacterEnemy* CurrentEnemy = EnemyList[CurrentNestedIndex];

            // Sphere - Sphere Collision
            FVector CollisionNormal = CurrentEnemy->Location - CurrentCharacterProjectile->Location; // not normalized yet
            const float DISTANCE = CollisionNormal.GetMagnitude();

            if (DISTANCE < (CurrentCharacterProjectile->Radius + CurrentEnemy->Radius))
            {
                // collision detected
                CurrentEnemy->GetDamage();

                CurrentCharacterProjectile->Die();
                break;
            }
        }
    }
}
