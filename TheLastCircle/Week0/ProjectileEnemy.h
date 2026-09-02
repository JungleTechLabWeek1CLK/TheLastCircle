#pragma once

#include "Projectile.h"
#include "Character.h"

class UProjectileEnemy : public UProjectile
{
public:
	UProjectileEnemy(float damage) : UProjectile(damage, ETypeProjectile::ETP_Projectile)
	{
		CharacterType = ETypeCharacter::ETC_EnemyProjectile;
	}
private:
};