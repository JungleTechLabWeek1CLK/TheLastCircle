#pragma once
#include "Physics.h"
#include "Renderer.h"

enum ETypeItem
{
	ETI_Bomb,
	ETI_heal,
	ETI_EXP,
	ETI_Magnet
};

class UItem
{
public:
	FVector Location;
	FVector Velocity;
	float Radius = 0.01f;
	float Mass;
	float Speed = 0.1f;
	ETypePrimitive HitBox;
	FVector Color;
	ETypeItem ItemType;
	UItem(FVector location = { 0,0,0 }, FVector velocity = { 0,0,0 })
	{
		Location = location;
		Velocity = velocity;
		
	}
	~UItem()
	{
	}
};