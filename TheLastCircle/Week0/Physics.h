#pragma once

#include "Math.h"
#include <cmath>
#include <Windows.h>

// Basic Directions
const FVector DIRECTION_LEFT(-1.f, 0.f, 0.f);
const FVector DIRECTION_RIGHT(1.f, 0.f, 0.f);
const FVector DIRECTION_UP(0.f, 1.f, 0.f);
const FVector DIRECTION_DOWN(0.f, -1.f, 0.f);


// Handle Collisions
class UCharacterPlayer;
class UCharacterEnemy;
class UProjectile;
void HandleCollision(UCharacterPlayer* Player, UCharacterEnemy** EnemyList, INT32 EnemyListCount,
    UProjectile** ProjectilePlayerList, INT32 ProjectilePlayerListCount, UProjectile** ProjectileEnemyList, INT32 ProjectileEnemyListCount, const float DELTA_TIME);