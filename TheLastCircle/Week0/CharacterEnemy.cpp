#include "CharacterEnemy.h"

void UCharacterEnemy::Move(FVector location, float delta) {
    UCharacter::Move(location, delta);
    float d = sqrt((Location.x - location.x) * (Location.x - location.x) + (Location.y - location.y) * (Location.y - location.y));
        if (d <= Range && Delay <= ShootTime)
            Attack(location);
}

void UCharacterEnemy::Attack(FVector location) {
    UCharacter::Attack(location);
    bIsShoot = true;
    ShootTime = 0;
}

void UCharacterEnemy::UpdateTime(float delta) {
    if (!bIsShoot) {
        ShootTime += delta;
    }
}