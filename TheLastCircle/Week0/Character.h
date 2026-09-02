#pragma once

#include "Physics.h"
#include "Renderer.h"


enum class ETypeCharacter
{
	ETC_Player,
	ETC_Enemy,
	ETC_PlayerProjectile,
	ETC_EnemyProjectile
};

class UCharacter
{
public:
	////////
	// Required for test
	FVector Location;
	FVector Velocity;
	float Radius = 0.05f;
	float Mass;
	float Hp;
	float Speed;
	float Damage;
	float Range;
	ETypePrimitive HitBox;
	FVector Color;
	ETypeCharacter CharacterType;

	float Delay;
	float ShootTime = 0;
	bool bIsShoot = true;

	float MinLocation = -5;
	float MaxLocation = 5;
	////////

	UCharacter(FVector location = {0,0,0}, FVector velocity = {0,0,0})
	{
		Location = location;
		Velocity = velocity;
		Damage = 100;
	}
	~UCharacter()
	{
	}

	virtual void Move(FVector location, float delta);
	virtual void Attack(FVector location);
	virtual void GetDamage(float damage);
	virtual void Die();
	bool IsActive();
	void UpdateTime(float delta);
private:
	bool bIsActive = true;
};
