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
	////////

	UCharacter(FVector location = {0,0,0}, FVector velocity = {0,0,0})
	{
		Location = location;
		Velocity = velocity;
	}
	~UCharacter()
	{
	}

	void Move(FVector location, float delta);
	void Attack(FVector location);
	void GetDamage();
	void Die();
private:

};
