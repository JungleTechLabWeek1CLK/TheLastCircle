#pragma once
#include "Physics.h"
#include "Renderer.h"

class UItem
{
public:
	FVector Location;
	FVector Velocity;
	float Radius = 0.01f;
	float Mass;
	float Speed;
	ETypePrimitive HitBox;
	FVector Color;
	UItem(FVector location = { 0,0,0 }, FVector velocity = { 0,0,0 })
	{
		Location = location;
		Velocity = velocity;
		
	}
	~UItem()
	{
	}
};