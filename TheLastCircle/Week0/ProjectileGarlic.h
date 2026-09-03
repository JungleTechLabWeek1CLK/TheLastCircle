#pragma once

#include "Projectile.h"

class UGameManager;
class UProjectileGarlic : public UProjectile
{
public:
	UProjectileGarlic(float damage, float radius) : UProjectile(damage, ETypeProjectile::ETP_Projectile, radius)
	{
		CharacterType = ETypeCharacter::ETC_PlayerProjectile;
	}

	virtual void CollisionCheck(UGameManager* GameManager) override;
private:
};