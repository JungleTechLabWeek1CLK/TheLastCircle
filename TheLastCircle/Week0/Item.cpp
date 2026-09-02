#include "Item.h"
#include "GameManager.h"

void UItem::CollisionCheck(UGameManager* gameManager) {
	UCharacterPlayer* Player = gameManager->GetPlayer();

	if (bIsActive == false)
		return;

	FVector CollisionNormal = Location - Player->Location; // not normalized yet
	const float DISTANCE = CollisionNormal.GetMagnitude();

	if (DISTANCE < (Player->Radius + Radius))
	{
		ItemEffect(gameManager);
	}
}