#pragma once

#include "Character.h"

class UProjectile : public UCharacter
{
public:
	UProjectile(float damage) {
		Radius = 0.001f;
		Speed = 1.f;
		Damage = damage;
	}
	void Move(FVector location, float delta)
	{
		if (bIsActive)
		{
			Location.x += Velocity.x * delta;
			Location.y += Velocity.y * delta;

			DealthTimer -= delta;
			if (DealthTimer < 0.f)
				Die();
		}
	}
	void Die()
	{
		bIsActive = false;
	}
	bool IsActive()
	{
		return bIsActive;
	}
private:
	float DealthTimer = 5.f;
	bool bIsActive = true;
};