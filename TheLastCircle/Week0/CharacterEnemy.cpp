#include "CharacterEnemy.h"
#include "SoundManager.h"

void UCharacterEnemy::GetDamage(float damage)
{
    UCharacter::GetDamage(damage);
    if(damage >10.f)
    USoundManager::GetInstance().PlaySFX(ESFXType::EnemyHit);
}


void UCharacterEnemy::Move(FVector location, float delta) {

    FVector PreviousLocation = Location;
    UCharacter::Move(location, delta);

    FVector TargetVelocity = Velocity;
    Location = PreviousLocation;

    // 현재 속도가 목표 속도를 천천히 따라가게 함
    InertiaVelocity.x +=
        (TargetVelocity.x - InertiaVelocity.x) * Acceleration * delta;

    InertiaVelocity.y +=
        (TargetVelocity.y - InertiaVelocity.y) * Acceleration * delta;


    // 관성이 적용된 속도로 실제 이동
    Location.x += InertiaVelocity.x * Speed * delta;
    Location.y += InertiaVelocity.y * Speed * delta;

    float d = sqrt((Location.x - location.x) * (Location.x - location.x) + (Location.y - location.y) * (Location.y - location.y));

    if (KnockbackTimer > 0.f)
    {
        Location.x += KnockbackDirection.x * 5.1f * delta;
        Location.y += KnockbackDirection.y * 5.1f * delta;
        KnockbackTimer -= delta;
    }
    else 
    {
        UCharacter::Move(location, delta);
        float d = sqrt((Location.x - location.x) * (Location.x - location.x) + (Location.y - location.y) * (Location.y - location.y));
        if (d <= Range && Delay <= ShootTime)
            Attack(location);
    }
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