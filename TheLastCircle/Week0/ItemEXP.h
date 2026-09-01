#pragma once
#include "Item.h"

class UItemEXP : public UItem
{
public:
	float Reward;
	float LootableRadius = Radius * 5.f;
	bool bIsFollow = false;
	bool bIsActive = true;
	UItemEXP(float reward) : UItem()
	{
		Reward = reward;
		ItemType = ETI_EXP;
	}
	void Die() {
		bIsActive = false;
	}
	bool IsActive() {
		return bIsActive;
	}
};