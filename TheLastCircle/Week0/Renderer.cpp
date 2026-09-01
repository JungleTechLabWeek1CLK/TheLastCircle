#include "Renderer.h"

#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"

void DrawCharacters(UCharacterPlayer* Player, UCharacterEnemy** EnemyList, INT32 EnemyListCount,
    UProjectile** ProjectilePlayerList, INT32 ProjectilePlayerListCount, UProjectile** ProjectileEnemyList, INT32 ProjectileEnemyListCount, URenderer* Renderer)
{
    // TODO: currently temporary color is being used
    FVector BallColor;

    BallColor = FVector(1.f, 1.0f, 1.0f);
    Renderer->UpdateConstantBuffer(Player->Location, Player->Radius, BallColor);
    Renderer->RenderPrimitive(EPT_Sphere);

    BallColor = FVector(1.f, 0.7f, 0.7f);
    for (INT32 CurrentIndex = 0; CurrentIndex < EnemyListCount; ++CurrentIndex)
    {
        Renderer->UpdateConstantBuffer(EnemyList[CurrentIndex]->Location, EnemyList[CurrentIndex]->Radius, BallColor);
        Renderer->RenderPrimitive(EPT_Triangle);
    }

    BallColor = FVector(0.f, 0.3f, 0.3f);
    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectilePlayerListCount; ++CurrentIndex)
    {
        Renderer->UpdateConstantBuffer(ProjectilePlayerList[CurrentIndex]->Location, ProjectilePlayerList[CurrentIndex]->Radius, BallColor);
        Renderer->RenderPrimitive(EPT_Sphere);
    }

    BallColor = FVector(0.5f, 0.7f, 0.3f);
    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileEnemyListCount; ++CurrentIndex)
    {
        Renderer->UpdateConstantBuffer(ProjectileEnemyList[CurrentIndex]->Location, ProjectileEnemyList[CurrentIndex]->Radius, BallColor);
        Renderer->RenderPrimitive(EPT_Sphere);
    }
}