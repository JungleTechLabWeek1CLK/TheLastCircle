#include "Renderer.h"

#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"
#include "ItemEXP.h"
#include "GameManager.h"

void DrawObjects(UCharacterPlayer* Player, UCharacterEnemy** EnemyList, INT32 EnemyListCount,
    UProjectile** ProjectileList, INT32 ProjectileListCount, UItemEXP** ItemEXPList, INT32 ItemEXPListCount,
    URenderer* Renderer, bool bIsTitle)
{
    if (bIsTitle)
        return;

    FVector PlayerOffset = Player->Location;
    Renderer->UpdateConstantBuffer(Player->Location - PlayerOffset, Player->Radius, Player->Location, 0.5f, Player->InvincibleTime);
    Renderer->RenderPrimitive(EPT_Sphere);

    for (INT32 CurrentIndex = 0; CurrentIndex < EnemyListCount; ++CurrentIndex)
    {
        Renderer->UpdateConstantBuffer(EnemyList[CurrentIndex]->Location - PlayerOffset, EnemyList[CurrentIndex]->Radius, Player->Location, 1.5f);
        Renderer->RenderPrimitive(EPT_Triangle);
    }

    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileListCount; ++CurrentIndex)
    {
        UProjectile* CurrentProjectile = ProjectileList[CurrentIndex];

        if (CurrentProjectile->IsActive() == false)
            continue;

        if(CurrentProjectile->CharacterType == ETypeCharacter::ETC_PlayerProjectile)
            Renderer->UpdateConstantBuffer(CurrentProjectile->Location - PlayerOffset, CurrentProjectile->Radius, Player->Location, 2.5f);
        else
            Renderer->UpdateConstantBuffer(CurrentProjectile->Location - PlayerOffset, CurrentProjectile->Radius, Player->Location, 3.5f);

        Renderer->RenderPrimitive(EPT_Sphere);
    }

    for (INT32 CurrentIndex = 0; CurrentIndex < ItemEXPListCount; ++CurrentIndex)
    {
        UItemEXP* CurrentItemEXP = ItemEXPList[CurrentIndex];
        if (CurrentItemEXP->IsActive() == false)
            continue;

        Renderer->UpdateConstantBuffer(CurrentItemEXP->Location - PlayerOffset, CurrentItemEXP->Radius, Player->Location, 4.5f);
        Renderer->RenderPrimitive(EPT_Sphere);
    }

    // TODO: drop item list will be added

}

void DrawBackground(UCharacterPlayer* Player, URenderer* Renderer, bool bIsTitle)
{
    if (bIsTitle)
        return;

    FVector Origin = { 0.f, 0.f, 0.f };
    Renderer->UpdateConstantBuffer(Origin, 1.f, Player->Location, 100.5f);
    Renderer->RenderPrimitive(EPT_BackgroundQuad);
}

void DrawUI(UCharacterPlayer* Player, URenderer* Renderer, bool bIsPlaying)
{
    if (bIsPlaying == false)
        return;

    // Health bar
    FVector Position = { -0.65f, 0.92f, 0.f};
    FVector Info = { Player->GetCurrentHp(), Player->GetMaxHp(), 0.f };
    Renderer->UpdateConstantBuffer(Position, 1.f, Info, 99.5f);
    Renderer->RenderPrimitive(EPT_UIQuad);

    // Exp bar
    Position = { 0.639f, 0.92f, 0.f };
    Info.x = Player->GetCurrentEXP();
    Info.y = Player->GetMaxEXP();
    Renderer->UpdateConstantBuffer(Position, 1.f, Info, 98.5f);
    Renderer->RenderPrimitive(EPT_UIQuad);
}