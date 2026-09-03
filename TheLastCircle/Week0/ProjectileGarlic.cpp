#include "ProjectileGarlic.h"
#include "GameManager.h"

void UProjectileGarlic::CollisionCheck(UGameManager* GameManager)
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
            CurrentEnemy->GetDamage(Damage);
        }
    }
}