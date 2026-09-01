#include "CharacterPlayer.h"

void UCharacterPlayer::UpdateTime(float delta) {
    if (!bIsShoot) {
        ShootTime += delta;
    }
    if (Delay <= ShootTime) {
        bIsShoot = true;
        ShootTime = 0;
    }
}