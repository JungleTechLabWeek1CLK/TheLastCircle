#include "Physics.h"
#include <Windows.h>


#include "CharacterPlayer.h"
#include "Character.h"



/*
필수적인 구현 내용

GetDamage -> 연속적으로 call이 되도, 자체적으로 일정 시간 후에만 처리되도록 해야함
Die -> projectile의 die가 예상한대로 잘 동작해야함
*/
void HandleCollision(UCharacterPlayer* Player, UCharacter** CharacterList, const float DELTA_TIME)
{
    UCharacter* CurrentCharacter = nullptr;

    // TODO: get this value from game manager
    INT32 TotalCharacterCount = 10;

    // detecting collision regarding player
    for (INT32 CurrentIndex = 0; CurrentIndex < TotalCharacterCount; ++CurrentIndex)
    {
        CurrentCharacter = CharacterList[CurrentIndex];
        if (CurrentCharacter->CharacterType == ETypeCharacter::ETC_Player || CurrentCharacter->CharacterType == ETypeCharacter::ETC_PlayerProjectile)
            continue;

        // Player - Enemy or Player - Enemy Projectile
        // Sphere - Sphere Collision
        FVector CollisionNormal = CurrentCharacter->Location - Player->Location; // not normalized yet
        const float DISTANCE = CollisionNormal.GetMagnitude();

        if (DISTANCE < (Player->Radius + CurrentCharacter->Radius))
        {
            // collision detected
            Player->GetDamage();

            if (CurrentCharacter->CharacterType == ETypeCharacter::ETC_EnemyProjectile)
            {
                CurrentCharacter->Die();
            }
        }
    }

    // detecting collision regarding enemy
    UCharacter* CurrentCharacterProjectile, * CurrentEnemy = nullptr;
    for (INT32 CurrentIndex = 0; CurrentIndex < TotalCharacterCount; ++CurrentIndex)
    {
        CurrentCharacterProjectile = CharacterList[CurrentIndex];
        if (CurrentCharacterProjectile->CharacterType == ETypeCharacter::ETC_PlayerProjectile)
        {
            for (INT32 CurrentNextIndex = CurrentIndex + 1; CurrentNextIndex < TotalCharacterCount; ++CurrentNextIndex)
            {
                CurrentEnemy = CharacterList[CurrentNextIndex];
                if (CurrentEnemy->CharacterType == ETypeCharacter::ETC_Enemy)
                {
                    // Player Projectile - Enemy
                    // Sphere - Sphere Collision
                    FVector CollisionNormal = CurrentEnemy->Location - CurrentCharacterProjectile->Location; // not normalized yet
                    const float DISTANCE = CollisionNormal.GetMagnitude();

                    if (DISTANCE < (CurrentCharacterProjectile->Radius + CurrentEnemy->Radius))
                    {
                        // collision detected
                        CurrentEnemy->GetDamage();

                        CurrentCharacterProjectile->Die();
                        break;
                    }
                }
            }
        }
    }
}
