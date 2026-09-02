#include "ItemMagnet.h"
#include "GameManager.h"

void UItemMagnet::ItemEffect(UGameManager* gameManager) {
    UCharacterEnemy** EnemyList = gameManager->GetEnemyList();
    INT32 EnemyListCount = gameManager->GetEnemyListCount();
    for (INT32 CurrentIndex = 0; CurrentIndex < EnemyListCount; ++CurrentIndex)
    {
        UCharacterEnemy* CurrentEnemy = EnemyList[CurrentIndex];
        CurrentEnemy->Die();
    }
    UItem::ItemEffect(gameManager);
}

void UItemMagnet::CollisionCheck(UGameManager* gameManager) {

}