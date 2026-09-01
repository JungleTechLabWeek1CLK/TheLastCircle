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

void UCharacterPlayer::LevelUp() {
    while (MaxEXP <= EXP) {
        Level++;
        EXP -= MaxEXP;
        MaxEXP *= 1.1f;
        Upgrade();
    }
}
void UCharacterPlayer::Upgrade() {
    Damage += 100;
}
void UCharacterPlayer::GetEXP(int exp) {
    EXP += exp;
    LevelUp();
}