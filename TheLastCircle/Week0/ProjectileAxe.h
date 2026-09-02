#pragma once

#include "Projectile.h"
#include "Character.h"

class UProjectileAxe : public UProjectile
{
public:
	int Penetration;
	int Cnt;
	UCharacterEnemy** HitEnemyList;
	UProjectileAxe(float damage, int cnt) : UProjectile(damage, ETypeProjectile::ETP_Axe)
	{
		CharacterType = ETypeCharacter::ETC_PlayerProjectile;
		Penetration = cnt;
		Cnt = 0;
		HitEnemyList = new UCharacterEnemy * [cnt]();
	}
	~UProjectileAxe()
	{
		delete[] HitEnemyList;
	}
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
private:
};