#include "ItemHeal.h"
#include "GameManager.h"

void UItemHeal::ItemEffect(UGameManager* gameManager) {
    UCharacterPlayer* Player = gameManager->GetPlayer();
    Player->Hp = min(Player->Hp + 100, Player->MaxHp);
    UItem::ItemEffect(gameManager);
}

void UItemHeal::CollisionCheck(UGameManager* gameManager) {
    UItem::CollisionCheck(gameManager);
}