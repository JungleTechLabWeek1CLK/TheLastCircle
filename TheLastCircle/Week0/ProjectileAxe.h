#pragma once

#include "Projectile.h"

class UGameManager;
class UCharacterEnemy;
class UProjectileAxe : public UProjectile
{
public:
	int Penetration;
	int Cnt;
	UCharacterEnemy** HitEnemyList;
    UProjectileAxe(float damage, int cnt);
	~UProjectileAxe();
	void Move(FVector location, float delta) override {
		Velocity.y -= 0.05f;

		if (IsActive())
		{
			Location.x += Velocity.x * delta;
			Location.y += Velocity.y * delta;
			float DealthTimer = GetDealthTimer();
			DealthTimer -= delta;
			if (DealthTimer < 0.f)
				Die();
			else
				SetDealthTimer(DealthTimer);
		}
	}

	virtual void CollisionCheck(UGameManager* GameManager) override;
private:
};