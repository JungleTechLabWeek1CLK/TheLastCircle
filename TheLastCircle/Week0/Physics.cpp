#include "Physics.h"
#include <Windows.h>

#include "Character.h"
#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"
#include "ItemEXP.h"
#include "GameManager.h"


void HandleCollision(UGameManager* gameManager, const float DELTA_TIME)
{
    if (gameManager->IsPlaying() == false)
        return;
    UCharacterPlayer* Player = gameManager->GetPlayer();
    UCharacterEnemy** EnemyList = gameManager->GetEnemyList();
    INT32 EnemyListCount = gameManager->GetEnemyListCount();
    UProjectile** ProjectileList = gameManager->GetProjectileList();
    INT32 ProjectileListCount = gameManager->GetProjectileListCount();
    UItemEXP** ItemEXPList = gameManager->GetEXPList();
    INT32 ItemEXPListCount = gameManager->GetEXPListCount();
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
            Player->GetDamage(CurrentEnemy->Damage);
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
            Player->GetDamage(CurrentEnemyProjectile->Damage);

            CurrentEnemyProjectile->Die();
        }
    }
    // Player - ItemEXP (magnetic check)
    for (INT32 CurrentIndex = 0; CurrentIndex < ItemEXPListCount; ++CurrentIndex)
    {
        UItemEXP* CurrentItemEXP = ItemEXPList[CurrentIndex];
        if (CurrentItemEXP->bIsActive == false || CurrentItemEXP->bIsFollow)
            continue;

        // Sphere - Sphere Collision
        FVector CollisionNormal = CurrentItemEXP->Location - Player->Location; // not normalized yet
        const float DISTANCE = CollisionNormal.GetMagnitude();

        if (DISTANCE < (Player->Radius + CurrentItemEXP->LootableRadius))
        {
            // collision detected
            CurrentItemEXP->bIsFollow = true;
        }
    }
    // Player - ItemEXP (collision check)
    for (INT32 CurrentIndex = 0; CurrentIndex < ItemEXPListCount; ++CurrentIndex)
    {
        UItemEXP* CurrentItemEXP = ItemEXPList[CurrentIndex];
        if (CurrentItemEXP->bIsActive == false || CurrentItemEXP->bIsFollow == false)
            continue;

        // Sphere - Sphere Collision
        FVector CollisionNormal = CurrentItemEXP->Location - Player->Location; // not normalized yet
        const float DISTANCE = CollisionNormal.GetMagnitude();

        if (DISTANCE < (Player->Radius + CurrentItemEXP->Radius))
        {
            // collision detected
            Player->GetEXP(CurrentItemEXP->Reward);

            CurrentItemEXP->ItemEffect(gameManager);
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
                CurrentEnemy->GetDamage(CurrentPlayerProjectile->Damage);

                CurrentPlayerProjectile->Die();
                break;
            }
        }
    }
}
