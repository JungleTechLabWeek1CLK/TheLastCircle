#pragma once

#include "Projectile.h"
#include "Character.h"

class UProjectilePlayer : public UProjectile
{
public:
	UProjectilePlayer() : UProjectile()
	{
		CharacterType = ETypeCharacter::ETC_EnemyProjectile;
	}
private:
};