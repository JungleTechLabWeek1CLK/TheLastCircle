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
class UItemEXP;
class UGameManager;
void HandleCollision(UGameManager* GameManager, const float DELTA_TIME);