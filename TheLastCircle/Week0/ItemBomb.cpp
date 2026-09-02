#include "ItemBomb.h"
#include "GameManager.h"

void UItemBomb::ItemEffect(UGameManager* gameManager) {
    IsExploded = true;
    UCharacterEnemy** EnemyList = gameManager->GetEnemyList();
    INT32 EnemyListCount = gameManager->GetEnemyListCount();
    Radius += gameManager->GetDeltaTime();
    for (INT32 CurrentIndex = 0; CurrentIndex < EnemyListCount; ++CurrentIndex)
    {
        UCharacterEnemy* CurrentEnemy = EnemyList[CurrentIndex];

        // Sphere - Sphere Collision
        FVector CollisionNormal = CurrentEnemy->Location - Location; // not normalized yet
        const float DISTANCE = CollisionNormal.GetMagnitude();

        if (DISTANCE < (Radius + CurrentEnemy->Radius))
        {
            CurrentEnemy->Die();
        }
    }
    if(Radius > BigRadius)
        UItem::ItemEffect(gameManager);
}

void UItemBomb::CollisionCheck(UGameManager* gameManager) {
    UItem::CollisionCheck(gameManager);
}