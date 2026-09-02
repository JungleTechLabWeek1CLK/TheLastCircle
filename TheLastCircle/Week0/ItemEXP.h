#pragma once
#include "Item.h"

class UGameManager;

class UItemEXP : public UItem
{
public:
	float Reward = 100;
	bool bIsFollow = false;
	UItemEXP() : UItem(ETypeItem::ETI_EXP)
	{
		BigRadius = Radius * 10.f;
		bIsMagnet = true;
	}
	void Move(FVector location, float delta);
	void ItemEffect(UGameManager* gameManager) override;
	void CollisionCheck(UGameManager* gameManager) override;
};