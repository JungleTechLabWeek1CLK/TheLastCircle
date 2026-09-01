#include "CharacterPlayer.h"

void UCharacterPlayer::UpdateTime(float delta) {
    
    ShootTime += delta;
    GuardTime += delta;
    if (bIsGuard) {
        InvincibleTime += delta;
    }
    if (bIsGuard && GuardDelay <= GuardTime) {
        bIsGuard = false;
        InvincibleTime = 0.f;
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
    switch (type)
    {
    case ETypeUpgrade::ETU_Damage:
        Damage += 10;
        break;
    case ETypeUpgrade::ETU_Hp:
        MaxHp += 10;
        Hp += 10;
        break;
    case ETypeUpgrade::ETU_bullets:
        Bullets++;
        break;
    case ETypeUpgrade::ETU_Speed:
        Speed += 0.05f;
        break;
    }
}
void UCharacterPlayer::GetEXP(int exp) {
    EXP += exp;
}

ETypeUpgrade* UCharacterPlayer::RandomUpgrade() {
    int size = static_cast<int>(ETypeUpgrade::COUNT);
    for (int i = 0; i < size; i++) {
        int r = i + rand() % (size - i);
        ETypeUpgrade temp = Upgrades[i];
        Upgrades[i] = Upgrades[r];
        Upgrades[r] = temp;
    }
    return Upgrades;
}