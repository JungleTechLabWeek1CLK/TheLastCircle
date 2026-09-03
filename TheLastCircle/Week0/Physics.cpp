#include "Physics.h"
#include <Windows.h>

#include "Character.h"
#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"
#include "ItemEXP.h"
#include "GameManager.h"


void HandleCollision(UGameManager* GameManager, const float DELTA_TIME)
{
    if (GameManager->IsPlaying() == false)
        return;

    UCharacterPlayer* Player = GameManager->GetPlayer();
    UCharacterEnemy** EnemyList = GameManager->GetEnemyList();
    INT32 EnemyListCount = GameManager->GetEnemyListCount();
    UProjectile** ProjectileList = GameManager->GetProjectileList();
    INT32 ProjectileListCount = GameManager->GetProjectileListCount();
    UItem** ItemList = GameManager->GetItemList();
    INT32 ItemListCount = GameManager->GetItemListCount();


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
    // ----------


    // Item (collision check)
    for (INT32 CurrentIndex = 0; CurrentIndex < ItemListCount; ++CurrentIndex)
    {
        UItem* CurrentItem = ItemList[CurrentIndex];
        CurrentItem->CollisionCheck(GameManager);
    }


    // Projectile (collision check)
    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileListCount; ++CurrentIndex)
    {
        UProjectile* CurrentProjectile = ProjectileList[CurrentIndex];
        CurrentProjectile->CollisionCheck(GameManager);
    }
}
