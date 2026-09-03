#include "ProjectileAxe.h"
#include "GameManager.h"


UProjectileAxe::UProjectileAxe(float damage, int cnt, float radius) : UProjectile(damage, ETypeProjectile::ETP_Axe, radius)
{
    CharacterType = ETypeCharacter::ETC_PlayerProjectile;
    Penetration = cnt;
    Cnt = 0;
    HitEnemyList = new UCharacterEnemy * [cnt]();
}

UProjectileAxe::~UProjectileAxe()
{
    delete[] HitEnemyList;
}

void UProjectileAxe::CollisionCheck(UGameManager* GameManager)
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
            if (Cnt != 0) {
                bool sw = true;
                for (int i = 0; i < Cnt; i++) {
                    if (HitEnemyList[i] == CurrentEnemy) {
                        sw = false;
                        break;
                    }
                }
                if (!sw)  break;
                Cnt++;
                CurrentEnemy->GetDamage(Damage);
                if (Cnt == Penetration)
                    Die();
                else {
                    HitEnemyList[Cnt - 1] = CurrentEnemy;
                }
            }
            else {
                Cnt++;
                CurrentEnemy->GetDamage(Damage);
                if (Cnt == Penetration)
                    Die();
                else {
                    HitEnemyList[Cnt - 1] = CurrentEnemy;
                }
                break;
            }
        }
    }

    /*
    for (INT32 CurrentNestedIndex = 0; CurrentNestedIndex < EnemyListCount; ++CurrentNestedIndex)
    {
        UCharacterEnemy* CurrentEnemy = EnemyList[CurrentNestedIndex];

        // Sphere - Sphere Collision
        FVector CollisionNormal = CurrentEnemy->Location - CurrentPlayerProjectile->Location; // not normalized yet
        const float DISTANCE = CollisionNormal.GetMagnitude();

        if (DISTANCE < (CurrentPlayerProjectile->Radius + CurrentEnemy->Radius))
        {
            // collision detected
            if (CurrentPlayerProjectile->ProjectileType == ETypeProjectile::ETP_Axe)
            {
                UProjectileAxe* PlayerProjectile = dynamic_cast<UProjectileAxe*>(CurrentPlayerProjectile);
                if (PlayerProjectile->Cnt != 0) {
                    bool sw = true;
                    for (int i = 0; i < PlayerProjectile->Cnt; i++) {
                        if (PlayerProjectile->HitEnemyList[i] == CurrentEnemy) {
                            sw = false;
                            break;
                        }
                    }
                    if (!sw)  break;
                    PlayerProjectile->Cnt++;
                    CurrentEnemy->GetDamage(CurrentPlayerProjectile->Damage);
                    if (PlayerProjectile->Cnt == PlayerProjectile->Penetration)
                        CurrentPlayerProjectile->Die();
                    else {
                        PlayerProjectile->HitEnemyList[PlayerProjectile->Cnt - 1] = CurrentEnemy;
                    }
                }
                else {
                    PlayerProjectile->Cnt++;
                    CurrentEnemy->GetDamage(CurrentPlayerProjectile->Damage);
                    if (PlayerProjectile->Cnt == PlayerProjectile->Penetration)
                        CurrentPlayerProjectile->Die();
                    else {
                        PlayerProjectile->HitEnemyList[PlayerProjectile->Cnt - 1] = CurrentEnemy;
                    }
                    break;
                }
            }
        }
    }
    */
}