#pragma once

#include "Projectile.h"

class UGameManager;
class UProjectileEnemy : public UProjectile
{
public:
	UProjectileEnemy(float damage) : UProjectile(damage, ETypeProjectile::ETP_Projectile)
	{
		CharacterType = ETypeCharacter::ETC_EnemyProjectile;
	}

	virtual void CollisionCheck(UGameManager* GameManager) override;
private:
};