#include "ItemEXP.h"
#include "GameManager.h"

void UItemEXP::Move(FVector location, float delta) {
    float d = sqrt((Location.x - location.x) * (Location.x - location.x) + (Location.y - location.y) * (Location.y - location.y));
    Velocity = { (location.x - Location.x) / d, (location.y - Location.y) / d, 0 };
    Location.x += Velocity.x * delta * Speed;
    Location.y += Velocity.y * delta * Speed;
    return;
}

void UItemEXP::ItemEffect(UGameManager* gameManager) {
    gameManager->GetPlayer()->GetEXP(this->Reward);
    UItem::ItemEffect(gameManager);
}