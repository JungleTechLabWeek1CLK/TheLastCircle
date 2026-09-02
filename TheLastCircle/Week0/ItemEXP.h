#pragma once
#include "Item.h"

class UGameManager;

class UItemEXP : public UItem
{
public:
	float Reward;
	float LootableRadius = Radius * 10.f;
	bool bIsFollow = false;
	UItemEXP(float reward) : UItem()
	{
		Reward = reward;
		ItemType = ETI_EXP;
	}
	void Move(FVector location, float delta);
	void ItemEffect(UGameManager* gameManager) override;
};