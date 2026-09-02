#pragma once
#include "Item.h"

class UGameManager;

class UItemBomb : public UItem
{
public:
	float Damage = 100;
	UItemBomb() : UItem(ETypeItem::ETI_Bomb)
	{
		Radius = 0.06f;
		BigRadius = Radius * 100.f;
		bIsMagnet = false;
	}
	void ItemEffect(UGameManager* gameManager) override;
	void CollisionCheck(UGameManager* gameManager) override;
};