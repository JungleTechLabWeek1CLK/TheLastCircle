#include "Renderer.h"

#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"

void DrawCharacters(UCharacterPlayer* Player, UCharacterEnemy** EnemyList, INT32 EnemyListCount,
    UProjectile** ProjectileList, INT32 ProjectileListCount, URenderer* Renderer, bool bIsTitle)
{
    if (bIsTitle)
        return;

    Renderer->UpdateConstantBuffer(Player->Location, Player->Radius, Player->Location, 0.5f);
    Renderer->RenderPrimitive(EPT_Sphere);

    for (INT32 CurrentIndex = 0; CurrentIndex < EnemyListCount; ++CurrentIndex)
    {
        Renderer->UpdateConstantBuffer(EnemyList[CurrentIndex]->Location, EnemyList[CurrentIndex]->Radius, Player->Location, 1.5f);
        Renderer->RenderPrimitive(EPT_Triangle);
    }

    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileListCount; ++CurrentIndex)
    {
        UProjectile* CurrentProjectile = ProjectileList[CurrentIndex];

        if (CurrentProjectile->IsActive() == false)
            continue;

        if(CurrentProjectile->CharacterType == ETypeCharacter::ETC_PlayerProjectile)
            Renderer->UpdateConstantBuffer(CurrentProjectile->Location, CurrentProjectile->Radius, Player->Location, 2.5f);
        else
            Renderer->UpdateConstantBuffer(CurrentProjectile->Location, CurrentProjectile->Radius, Player->Location, 3.5f);

        Renderer->RenderPrimitive(EPT_Sphere);
    }
}