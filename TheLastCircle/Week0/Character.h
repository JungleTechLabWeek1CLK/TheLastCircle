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
enum class ETypeProjectile
{
	ETP_Projectile,
	ETP_Axe,
	ETP_Bible
};
class UCharacter
{
public:
	////////
	// Required for test
	FVector Location;
	FVector Velocity;
	float Radius;
	float Mass;
	float Hp;
	float Speed;
	float Damage;
	float Range;
	ETypePrimitive HitBox;
	FVector Color;
	ETypeCharacter CharacterType;

	// knockback
	FVector KnockbackDirection;
	float KnockbackTimer = -1.f;


	float Delay;
	float ShootTime = 0;
	bool bIsShoot = true;

	float MinLocation = -3;
	float MaxLocation = 3;
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

	void GetKnockback(FVector InputKnockbackDirection)
	{
		KnockbackDirection = InputKnockbackDirection;
		KnockbackTimer = 0.15f;
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
