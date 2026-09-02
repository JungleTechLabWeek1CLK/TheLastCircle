#pragma once
#include "Item.h"

class UGameManager;

class UItemHeal : public UItem
{
public:
	UItemHeal() : UItem(ETypeItem::ETI_Heal)
	{
		BigRadius = 0;
		bIsMagnet = false;
	}
	void ItemEffect(UGameManager* gameManager) override;
	void CollisionCheck(UGameManager* gameManager) override;
};