#include "Physics.h"
#include <Windows.h>

#include "Character.h"
#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"



void HandleCollision(UCharacterPlayer* Player, UCharacterEnemy** EnemyList, INT32 EnemyListCount,
    UProjectile** ProjectileList, INT32 ProjectileListCount, const float DELTA_TIME)
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
    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileListCount; ++CurrentIndex)
    {
        UProjectile* CurrentEnemyProjectile = ProjectileList[CurrentIndex];
        if (CurrentEnemyProjectile->CharacterType == ETypeCharacter::ETC_PlayerProjectile || CurrentEnemyProjectile->IsActive() == false)
            continue;

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
    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileListCount; ++CurrentIndex)
    {
        UProjectile* CurrentPlayerProjectile = ProjectileList[CurrentIndex];
        if (CurrentPlayerProjectile->CharacterType == ETypeCharacter::ETC_EnemyProjectile || CurrentPlayerProjectile->IsActive() == false)
            continue;

        for (INT32 CurrentNestedIndex = 0; CurrentNestedIndex < EnemyListCount; ++CurrentNestedIndex)
        {
            UCharacterEnemy* CurrentEnemy = EnemyList[CurrentNestedIndex];

            // Sphere - Sphere Collision
            FVector CollisionNormal = CurrentEnemy->Location - CurrentPlayerProjectile->Location; // not normalized yet
            const float DISTANCE = CollisionNormal.GetMagnitude();

            if (DISTANCE < (CurrentPlayerProjectile->Radius + CurrentEnemy->Radius))
            {
                // collision detected
                CurrentEnemy->GetDamage();

                CurrentPlayerProjectile->Die();
                break;
            }
        }
    }
}
