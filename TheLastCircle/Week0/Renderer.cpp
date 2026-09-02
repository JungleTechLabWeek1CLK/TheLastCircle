#include "Renderer.h"

#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"
#include "ItemEXP.h"
#include "GameManager.h"

#include <algorithm>


enum RenderType
{
    PLAYER = 0, PROJECTILE_PLAYER = 2, PROJECTILE_ENEMY = 3, ITEM_EXP = 4,
    ENEMY_WALKER = 10, ENEMY_RUNNER, ENEMY_RANGER,
    UI_EXP = 98, UI_HEALTH,
    BACKGROUND = 100
};
constexpr float RENDER_OFFSET = 0.5f;

void DrawObjects(UGameManager* GameManager, URenderer* Renderer, bool bIsTitle)
{
    if (bIsTitle)
        return;

    UCharacterPlayer* Player = GameManager->GetPlayer();
    UCharacterEnemy** EnemyList = GameManager->GetEnemyList();
    INT32 EnemyListCount = GameManager->GetEnemyListCount();
    UProjectile** ProjectileList = GameManager->GetProjectileList();
    INT32 ProjectileListCount = GameManager->GetProjectileListCount();
    UItem** ItemList = GameManager->GetItemList();
    INT32 ItemListCount = GameManager->GetItemListCount();

    FVector CameraLocation = Renderer->UpdateCameraLocation(Player->Location, Player->MinLocation, Player->MaxLocation);

    Player->PlayerOffset = Player->Location - CameraLocation;

    Renderer->UpdateConstantBuffer(Player->PlayerOffset, Player->Radius, Player->Location, RenderType::PLAYER + RENDER_OFFSET, Player->InvincibleTime);
    Renderer->RenderPrimitive(EPT_Sphere);

    for (INT32 CurrentIndex = 0; CurrentIndex < EnemyListCount; ++CurrentIndex)
    {
        UCharacterEnemy* CurrentEnemy = EnemyList[CurrentIndex];
        float RenderType = -1.f;
        switch (CurrentEnemy->EnemyType)
        {
        case ETypeEnemy::ETE_Walker: RenderType = RenderType::ENEMY_WALKER + RENDER_OFFSET; break;
        case ETypeEnemy::ETE_Runner: RenderType = RenderType::ENEMY_RUNNER + RENDER_OFFSET; break;
        case ETypeEnemy::ETE_Ranger: RenderType = RenderType::ENEMY_RANGER + RENDER_OFFSET; break;
        }
        Renderer->UpdateConstantBuffer(CurrentEnemy->Location - CameraLocation, CurrentEnemy->Radius, Player->PlayerOffset, RenderType);
        Renderer->RenderPrimitive(EPT_Triangle);
    }

    for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileListCount; ++CurrentIndex)
    {
        UProjectile* CurrentProjectile = ProjectileList[CurrentIndex];

        if (CurrentProjectile->IsActive() == false)
            continue;

        if (CurrentProjectile->CharacterType == ETypeCharacter::ETC_PlayerProjectile)
            Renderer->UpdateConstantBuffer(CurrentProjectile->Location - CameraLocation, CurrentProjectile->Radius, Player->PlayerOffset, RenderType::PROJECTILE_PLAYER + RENDER_OFFSET);
        else
            Renderer->UpdateConstantBuffer(CurrentProjectile->Location - CameraLocation, CurrentProjectile->Radius, Player->PlayerOffset, RenderType::PROJECTILE_ENEMY + RENDER_OFFSET);

        Renderer->RenderPrimitive(EPT_Sphere);
    }

    for (INT32 CurrentIndex = 0; CurrentIndex < ItemListCount; ++CurrentIndex)
    {
        UItem* CurrentItemEXP = ItemList[CurrentIndex];
        if (CurrentItemEXP->IsActive() == false)
            continue;

        Renderer->UpdateConstantBuffer(CurrentItemEXP->Location - CameraLocation, CurrentItemEXP->Radius, Player->PlayerOffset, RenderType::ITEM_EXP + RENDER_OFFSET);
        Renderer->RenderPrimitive(EPT_Sphere);
    }
}

void DrawBackground(UCharacterPlayer* Player, URenderer* Renderer, bool bIsTitle)
{
    if (bIsTitle)
        return;

    FVector CameraLocation = Renderer->UpdateCameraLocation(Player->Location, Player->MinLocation, Player->MaxLocation);

    FVector Origin = { 0.f, 0.f, 0.f };
    Renderer->UpdateConstantBuffer(Origin, 1.f, CameraLocation, RenderType::BACKGROUND + RENDER_OFFSET);
    Renderer->RenderPrimitive(EPT_BackgroundQuad);
}



void DrawUI(UCharacterPlayer* Player, URenderer* Renderer, bool bIsPlaying)
{
    if (bIsPlaying == false)
        return;

    // Health bar
    FVector Position = { -0.65f, 0.92f, 0.f };
    FVector Info = { Player->GetCurrentHp(), Player->GetMaxHp(), 0.f };
    Renderer->UpdateConstantBuffer(Position, 1.f, Info, RenderType::UI_HEALTH + RENDER_OFFSET);
    Renderer->RenderPrimitive(EPT_UIQuad);

    // Exp bar
    Position = { 0.639f, 0.92f, 0.f };
    Info.x = Player->GetCurrentEXP();
    Info.y = Player->GetMaxEXP();
    Renderer->UpdateConstantBuffer(Position, 1.f, Info, RenderType::UI_EXP + RENDER_OFFSET);
    Renderer->RenderPrimitive(EPT_UIQuad);
}