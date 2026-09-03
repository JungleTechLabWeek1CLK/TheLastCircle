#pragma once

#include "Projectile.h"
#include <vector>

class UGameManager;
class UCharacterEnemy;
class UProjectileBible : public UProjectile
{
public:
	std::vector<UCharacterEnemy*> HitEnemyList;
	UProjectileBible(float InputDamage);
	~UProjectileBible();
	void Move(FVector PlayerLocation, float DeltaTime) override;

	virtual void CollisionCheck(UGameManager* GameManager) override;
private:

	float OrbitRadius = 0.17f;
	float CurrentAngle = 0.0f;
	float RotationSpeed = 5.1f;
};