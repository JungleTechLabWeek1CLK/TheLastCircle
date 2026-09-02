#pragma once
#include "Physics.h"
#include "Renderer.h"

class UGameManager;

class UItem
{
public:
	FVector Location;
	FVector Velocity;
	float Radius = 0.01f;
	float Mass;
	float Speed = 0.1f;
	float BigRadius;
	ETypePrimitive HitBox;
	FVector Color;
	
	bool bIsActive = true;
	bool bIsMagnet;
	UItem( FVector location = { 0,0,0 }, FVector velocity = { 0,0,0 })
	{
		Location = location;
		Velocity = velocity;
	}
	~UItem()
	{
	};
	void Die() {
		bIsActive = false;
	}
	bool IsActive() {
		return bIsActive;
	}

	virtual void ItemEffect(UGameManager* gameManager) {
		this->Die();
	}

	virtual void CollisionCheck(UGameManager* gameManager);
};