#include "ProjectileBible.h"
#include "GameManager.h"
#include <algorithm>

const float PI_X2 = 6.283185f;

UProjectileBible::UProjectileBible(float InputDamage) : UProjectile(InputDamage, ETypeProjectile::ETP_Bible), OrbitRadius(0.17f), CurrentAngle(0.0f), RotationSpeed(5.1f)
{
    CharacterType = ETypeCharacter::ETC_PlayerProjectile;
    Radius = 0.03f;
}

UProjectileBible::~UProjectileBible()
{
}

void UProjectileBible::Move(FVector PlayerLocation, float DeltaTime) 
{
    if (IsActive())
    {
        CurrentAngle += RotationSpeed * DeltaTime;
        if (CurrentAngle > PI_X2)
        {
            CurrentAngle -= PI_X2;
            HitEnemyList.clear();
        }

        Location.x = PlayerLocation.x + OrbitRadius * std::cos(CurrentAngle);
        Location.y = PlayerLocation.y + OrbitRadius * std::sin(CurrentAngle);

        /*
        float DealthTimer = GetDealthTimer();
        DealthTimer -= delta;
        if (DealthTimer < 0.f)
            Die();
            */
    }
}

void UProjectileBible::CollisionCheck(UGameManager* GameManager)
{
    if (IsActive() == false)
        return;

    UCharacterEnemy** EnemyList = GameManager->GetEnemyList();
    INT32 EnemyListCount = GameManager->GetEnemyListCount();

    for (INT32 CurrentNestedIndex = 0; CurrentNestedIndex < EnemyListCount; ++CurrentNestedIndex)
    {
        UCharacterEnemy* CurrentEnemy = EnemyList[CurrentNestedIndex];

        // Sphere - Sphere Collision
        FVector CollisionNormal = CurrentEnemy->Location - Location; // not normalized yet
        const float DISTANCE = CollisionNormal.GetMagnitude();

        if (DISTANCE < (Radius + CurrentEnemy->Radius))
        {
            // collision detected
            auto Result = std::find(HitEnemyList.cbegin(), HitEnemyList.cend(), CurrentEnemy);
            if (Result == HitEnemyList.cend())
            {
                // first hit
                CurrentEnemy->GetDamage(Damage);

                FVector DirPlayerToEnemy = CurrentEnemy->Location - GameManager->GetPlayer()->Location;
                DirPlayerToEnemy.Normalize();

                DirPlayerToEnemy = DirPlayerToEnemy * 0.1f;

                CurrentEnemy->GetKnockback(DirPlayerToEnemy);

                HitEnemyList.emplace_back(CurrentEnemy);
            }
        }
    }
}