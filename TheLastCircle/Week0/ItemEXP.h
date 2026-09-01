#pragma once
#include "Item.h"

class UItemEXP : public UItem
{
public:
	float Reword;
	UItemEXP(float reword) : UItem()
	{
		Reword = reword;
	}
};