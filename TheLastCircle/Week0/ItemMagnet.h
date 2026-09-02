#pragma once
#include "Item.h"

class UGameManager;

class UItemMagnet : public UItem
{
public:
	UItemMagnet() : UItem(ETypeItem::ETI_Magnet)
	{
		Radius = 0.06f;
		BigRadius = 0;
		bIsMagnet = false;
	}
	void ItemEffect(UGameManager* gameManager) override;
	void CollisionCheck(UGameManager* gameManager) override;
};