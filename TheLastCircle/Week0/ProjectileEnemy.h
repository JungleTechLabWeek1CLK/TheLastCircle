#pragma once

#include "Projectile.h"
#include "Character.h"

class UProjectileEnemy : public UProjectile
{
public:
	UProjectileEnemy() : UProjectile()
	{
		CharacterType = ETypeCharacter::ETC_EnemyProjectile;
	}
private:
};