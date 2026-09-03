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

	void ResetProjectile(int penetration)
	{
		Cnt = 0;
		Penetration = penetration;

		for (int i = 0; i < Penetration; ++i)
		{
			HitEnemyList[i] = nullptr;
		}
	}

	virtual void CollisionCheck(UGameManager* GameManager) override;
private:
};