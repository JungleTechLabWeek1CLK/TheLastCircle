#pragma once
#include "Item.h"

class UGameManager;

class UItemHeal : public UItem
{
public:
	UItemHeal() : UItem()
	{
		BigRadius = 0;
		bIsMagnet = false;
	}
	void ItemEffect(UGameManager* gameManager) override;
	void CollisionCheck(UGameManager* gameManager) override;
};