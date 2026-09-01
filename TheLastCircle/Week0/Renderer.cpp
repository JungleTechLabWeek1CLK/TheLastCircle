#include "Renderer.h"

#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"


void DrawCharacters(UCharacterPlayer* Player, UCharacterEnemy** EnemyList, INT32 EnemyListCount,
    UProjectile** ProjectileList, INT32 ProjectileListCount, URenderer* Renderer)
{
    // TODO: currently temporary color is being used
    FVector BallColor;

    BallColor = FVector(1.f, 1.0f, 1.0f);
    Renderer->UpdateConstantBuffer(Player->Location, Player->Radius, BallColor, Player->Location);
    Renderer->RenderPrimitive(EPT_Sphere);

    BallColor = FVector(1.f, 0.7f, 0.7f);
    for (INT32 CurrentIndex = 0; CurrentIndex < EnemyListCount; ++CurrentIndex)
    {
        Renderer->UpdateConstantBuffer(EnemyList[CurrentIndex]->Location, EnemyList[CurrentIndex]->Radius, BallColor, Player->Location, 1);
        Renderer->RenderPrimitive(EPT_Triangle);
    }

    BallColor = FVector(0.f, 0.3f, 0.3f);
    FVector BallColorEnemyProjectile = FVector(0.5f, 0.7f, 0.3);
    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileListCount; ++CurrentIndex)
    {
        UProjectile* CurrentProjectile = ProjectileList[CurrentIndex];

        if (CurrentProjectile->IsActive() == false)
            continue;

        if(CurrentProjectile->CharacterType == ETypeCharacter::ETC_PlayerProjectile)
            Renderer->UpdateConstantBuffer(CurrentProjectile->Location, CurrentProjectile->Radius, BallColor, Player->Location);
        else
            Renderer->UpdateConstantBuffer(CurrentProjectile->Location, CurrentProjectile->Radius, BallColorEnemyProjectile, Player->Location);

        Renderer->RenderPrimitive(EPT_Sphere);
    }
}