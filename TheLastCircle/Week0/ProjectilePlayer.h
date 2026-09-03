#pragma once

#include "Projectile.h"

class UGameManager;
class UCharacterEnemy;
class UProjectilePlayer : public UProjectile
{
public:
	int Penetration;
	int Cnt;
	UCharacterEnemy** HitEnemyList;
	UProjectilePlayer(float damage, int cnt);
	~UProjectilePlayer();

	virtual void CollisionCheck(UGameManager* GameManager) override;
private:
};