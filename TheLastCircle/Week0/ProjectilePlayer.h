#pragma once

#include "Projectile.h"
#include "Character.h"

class UProjectilePlayer : public UProjectile
{
public:
	int Penetration;
	int Cnt;
	UCharacterEnemy** HitEnemyList;
	UProjectilePlayer(float damage, int cnt) : UProjectile(damage, ETypeProjectile::ETP_Projectile)
	{
		CharacterType = ETypeCharacter::ETC_PlayerProjectile;
		Penetration = cnt;
		Cnt = 0;
		HitEnemyList = new UCharacterEnemy * [cnt]();
	}
	~UProjectilePlayer()
	{
		delete[] HitEnemyList;
	}
private:
};