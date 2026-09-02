#include "ItemBomb.h"
#include "GameManager.h"

void UItemBomb::ItemEffect(UGameManager* gameManager) {
    UCharacterEnemy** EnemyList = gameManager->GetEnemyList();
    INT32 EnemyListCount = gameManager->GetEnemyListCount();
    for (INT32 CurrentIndex = 0; CurrentIndex < EnemyListCount; ++CurrentIndex)
    {
        UCharacterEnemy* CurrentEnemy = EnemyList[CurrentIndex];

        // Sphere - Sphere Collision
        FVector CollisionNormal = CurrentEnemy->Location - Location; // not normalized yet
        const float DISTANCE = CollisionNormal.GetMagnitude();

        if (DISTANCE < (BigRadius + CurrentEnemy->Radius))
        {
            CurrentEnemy->Die();
        }
    }
    UItem::ItemEffect(gameManager);
}

void UItemBomb::CollisionCheck(UGameManager* gameManager) {
    UItem::CollisionCheck(gameManager);
}