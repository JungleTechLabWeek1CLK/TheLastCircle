#include "ProjectileEnemy.h"
#include "GameManager.h"


void UProjectileEnemy::CollisionCheck(UGameManager* GameManager) 
{
    if (IsActive() == false)
        return;

    // Sphere - Sphere Collision
    UCharacterPlayer* Player = GameManager->GetPlayer();

    FVector CollisionNormal = Location - Player->Location; // not normalized yet
    const float DISTANCE = CollisionNormal.GetMagnitude();

    if (DISTANCE < (Player->Radius + Radius))
    {
        // collision detected
        Player->GetDamage(Damage);

        Die();
    }
}