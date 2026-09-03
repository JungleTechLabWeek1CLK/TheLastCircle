#pragma once

#include "Character.h"

class UProjectile : public UCharacter
{
public:
	UProjectile(float damage, ETypeProjectile projectileType) {
		Radius = 0.01f;
		Speed = 1.f;
		Damage = damage;
		ProjectileType = projectileType;
	}
	virtual void Move(FVector location, float delta)
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
	float GetDealthTimer() {
		return DealthTimer;
	}
	void SetDealthTimer(float dealthTimer) {
		DealthTimer = dealthTimer;
	}

	virtual void CollisionCheck(UGameManager* GameManager) = 0;

	ETypeProjectile ProjectileType;

	void SetActive(bool bActive)
	{
		bIsActive = bActive;
	}
private:
	float DealthTimer = 5.f;
	bool bIsActive = true;
};