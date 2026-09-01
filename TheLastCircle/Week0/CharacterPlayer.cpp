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

int UCharacterPlayer::LevelUp() {
    int cnt = 0;
    while (MaxEXP <= EXP) {
        cnt++;
        Level++;
        EXP -= MaxEXP;
        MaxEXP *= 1.1f;
    }
    return cnt;
}
void UCharacterPlayer::Upgrade(ETypeUpgrade type) {
    Damage += 100;
}
void UCharacterPlayer::GetEXP(int exp) {
    EXP += exp;
}