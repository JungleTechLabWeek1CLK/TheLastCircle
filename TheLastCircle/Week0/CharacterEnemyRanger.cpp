#include "CharacterEnemyRanger.h"

void UCharacterEnemyRanger::Move(FVector location, float delta) {
    UpdateTime(delta);
}
void UCharacterEnemyRanger::UpdateTime(float delta) {
    if (!bIsShoot) {
        ShootTime += delta;
    }
    if (Delay <= ShootTime) {
        bIsShoot = true;
        ShootTime = 0;
    }
}