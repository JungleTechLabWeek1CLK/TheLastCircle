#pragma once

#include "Projectile.h"
#include "Character.h"

class UProjectilePlayer : public UProjectile
{
public:
	UProjectilePlayer(float damage) : UProjectile(damage)
	{
		CharacterType = ETypeCharacter::ETC_PlayerProjectile;
	}
private:
};