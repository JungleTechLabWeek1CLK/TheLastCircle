#include "CharacterPlayer.h"

void UCharacterPlayer::UpdateTime(float delta) {
    
    ShootTime += delta;
    GuardTime += delta;
    if (GuardDelay <= GuardTime) {
        bIsGuard = false;
    }
    if (Delay <= ShootTime) {
        bIsShoot = true;
        ShootTime = 0;
    }
}

void UCharacterPlayer::GetDamage(float damage) {
    if (!bIsGuard) {
        UCharacter::GetDamage(damage);
        bIsGuard = true;
        GuardTime = 0;
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