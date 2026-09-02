#include "ItemMagnet.h"
#include "GameManager.h"

void UItemMagnet::ItemEffect(UGameManager* gameManager) {
    UItem** ItemList = gameManager->GetItemList();
    INT32 ItemListCount = gameManager->GetItemListCount();
    for (INT32 CurrentIndex = 0; CurrentIndex < ItemListCount; ++CurrentIndex)
    {
        UItem* CurrentItem = ItemList[CurrentIndex];
        if (CurrentItem->bIsMagnet) {
            static_cast<UItemEXP*>(CurrentItem)->bIsFollow = true;
            CurrentItem->Speed = 1.f;
        }
    }
    UItem::ItemEffect(gameManager);
}

void UItemMagnet::CollisionCheck(UGameManager* gameManager) {
    UItem::CollisionCheck(gameManager);
}