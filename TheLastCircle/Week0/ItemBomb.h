#pragma once
#include "Item.h"

class UGameManager;

class UItemBomb : public UItem
{
public:
	float Damage = 100;
	bool IsExploded = false;

	UItemBomb() : UItem(ETypeItem::ETI_Bomb)
	{
		Radius = 0.06f;
		BigRadius = Radius * 7.5f;
		bIsMagnet = false;
	}
	~UItemBomb()
	{
	};
	void ItemEffect(UGameManager* gameManager) override;
	void CollisionCheck(UGameManager* gameManager) override;
};