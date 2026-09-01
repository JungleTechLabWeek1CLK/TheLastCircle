#include "Renderer.h"

#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"
#include "ItemEXP.h"

void DrawObjects(UCharacterPlayer* Player, UCharacterEnemy** EnemyList, INT32 EnemyListCount,
    UProjectile** ProjectileList, INT32 ProjectileListCount, UItemEXP** ItemEXPList, INT32 ItemEXPListCount,
    URenderer* Renderer, bool bIsTitle)
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

    for (INT32 CurrentIndex = 0; CurrentIndex < ItemEXPListCount; ++CurrentIndex)
    {
        UItemEXP* CurrentItemEXP = ItemEXPList[CurrentIndex];
        if (CurrentItemEXP->IsActive() == false)
            continue;

        Renderer->UpdateConstantBuffer(CurrentItemEXP->Location, CurrentItemEXP->Radius, Player->Location, 4.5f);
        Renderer->RenderPrimitive(EPT_Sphere);
    }

    // TODO: drop item list will be added

}

void DrawBackground(URenderer* Renderer, bool bIsTitle)
{
    if (bIsTitle)
        return;

    FVector Temp = { 0.f, 0.f, 0.f };
    Renderer->UpdateConstantBuffer(Temp, 1.f, Temp, 100.5f);
    Renderer->RenderPrimitive(EPT_Quad);
}