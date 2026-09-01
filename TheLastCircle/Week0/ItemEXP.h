#pragma once
#include "Item.h"

class UItemEXP : public UItem
{
public:
	float Reword;
	bool bIsFollow = false;
	bool bIsActive = true;
	UItemEXP(float reword) : UItem()
	{
		Reword = reword;
		ItemType = ETI_EXP;
	}
	void Die() {
		bIsActive = false;
	}
	bool IsActive() {
		return bIsActive;
	}
};