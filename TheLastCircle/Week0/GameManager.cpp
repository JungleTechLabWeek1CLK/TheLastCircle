#include "GameManager.h"
#include <cstdlib>
#include <cmath>


#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"
#include "Renderer.h"

UGameManager::UGameManager()
    : currentState(Title), 
    GameTime(0.0f), 
    GameClearTime(60.0f), 
    EnemyList(nullptr),EnemyCount(0), 
    EnemyCapacity(30), EnemySpawnTimer(0.0f), 
    EnemySpawnInterval(1.0f),
    ProjectileList(nullptr),
    ProjectileCount(0),
    ProjectileCapacity(30),
    ItemList(nullptr), ItemCount(0),
    ItemCapacity(30)
{

}


UGameManager::~UGameManager()
{
    ClearEnemies();
    ClearProjectiles();

    delete[] EnemyList;
    EnemyList = nullptr;

    delete[] ProjectileList;
    ProjectileList = nullptr;
}

void UGameManager::Initialize(URenderer* InputRenderer)
{
    USoundManager::GetInstance().Initialize();

    // 사운드 추가
    
    USoundManager::GetInstance().LoadSFX(ESFXType::PlayerShoot, L"Sound/PlayerShoot.wav");
    USoundManager::GetInstance().LoadSFX(ESFXType::PlayerHit, L"Sound/PlayerHit.wav");
    USoundManager::GetInstance().LoadSFX(ESFXType::PlayerDie, L"Sound/PlayerDie.wav");

    USoundManager::GetInstance().LoadSFX(ESFXType::EnemyHit, L"Sound/EnemyHit.wav");
    USoundManager::GetInstance().LoadSFX(ESFXType::EnemyDie, L"Sound/EnemyDie.wav");


    USoundManager::GetInstance().LoadSFX(ESFXType::LevelUp, L"Sound/LevelUp.wav");
    USoundManager::GetInstance().LoadSFX(ESFXType::Coin, L"Sound/Coin.wav");

    USoundManager::GetInstance().LoadSFX(ESFXType::GameClear, L"Sound/GameClear.wav");
    USoundManager::GetInstance().LoadSFX(ESFXType::GameOver, L"Sound/GameOver1.wav");

    USoundManager::GetInstance().LoadBGM(L"Sound/MainBGM.wav");
    //

    currentState = Title;

    Score = 0.f;
    KillCount = 0;
    GameTime = 0.0f;
    EnemyCount = 0;
    EnemyCapacity = 30;
    EnemyList = new UCharacterEnemy * [EnemyCapacity]();

    ProjectileCount = 0;
    ProjectileCapacity = 30;
    ProjectileList = new UProjectile * [ProjectileCapacity]();

    ItemCount = 0;
    ItemCapacity = 30;
    ItemList = new UItem * [ItemCapacity]();
    // 60초 생존하면 게임 클리어

    Renderer = InputRenderer;
}

void UGameManager::Update(float DeltaTime)
{
    USoundManager::GetInstance().Update();

    if (currentState != Playing)
    {
        return;
    }

    UpdateGameTime(DeltaTime);
    UpdateEnemySpawn(DeltaTime);
    //UpdateProjectiles(DeltaTime);

    UpdateScore();
    CheckGameOver();
    CheckGameClear();
}


#pragma region GameStateFunction

void UGameManager::ResetGame()
{
    GameTime = 0.0f;
    EnemySpawnTimer = 0.0f;
    Score = 0;

    KillCount = 0;

    currentState = Playing;

    ClearEnemies();
    ClearProjectiles();
    ClearItem();
    SpawnPlayer();

    USoundManager::GetInstance().PlayBGM();
    Renderer->ResetCameraLocation();
}

void UGameManager::PauseGame()
{
    if (currentState == Playing)
    {
        currentState = Paused;

        USoundManager::GetInstance().PauseBGM();
    }
}

void UGameManager::ResumeGame()
{
    if (currentState == Paused)
    {
        currentState = Playing;
        USoundManager::GetInstance().ResumeBGM();
    }
    else if (currentState == Upgrade)
    {
        currentState = Playing;
        USoundManager::GetInstance().ResumeBGM();
    }
}

void UGameManager::Lose()
{
    if (currentState != Playing)
    {
        return;
    }

    currentState = GameOver;

    USoundManager::GetInstance().StopBGM();

    USoundManager::GetInstance().PlaySFX(ESFXType::GameOver);
}

void UGameManager::Win()
{
    if (currentState != Playing)
    {
        return;
    }

    currentState = GameClear;

    USoundManager::GetInstance().StopBGM();

    USoundManager::GetInstance().PlaySFX(ESFXType::GameClear);
}

void UGameManager::ReturnToTitle()
{
    ClearEnemies();
    ClearProjectiles();

    GameTime = 0.0f;
    EnemySpawnTimer = 0.0f;
    Score = 0;

    currentState = Title;

    USoundManager::GetInstance().StopBGM();
}

#pragma endregion

bool UGameManager::IsTitle() const
{
    return currentState == Title;
}

bool UGameManager::IsPlaying() const
{
    return (currentState == Playing) ? true : false;
}

bool UGameManager::IsPaused() const
{
    return (currentState == Paused) ? true : false;
}

bool UGameManager::IsUpgrade() const
{
    return (currentState == Upgrade) ? true : false;
}

void UGameManager::AddKill()
{
    ++KillCount;
}

#pragma region SpawnFunction

void UGameManager::SpawnPlayer()
{
    FVector SpawnPosition(0.0f, 0.0f, 0.0f);

    Player.Location = SpawnPosition;
    Player.MaxHp = 1000.f;
    Player.Hp = Player.MaxHp;
    Player.MaxEXP = 100.f;
    Player.EXP = 0.f;
    Player.Bullets = 1;
    Player.Speed = 0.6f;
    Player.Damage = 100.f;
    Player.InvincibleTime = 0.f;
    Player.Radius = 0.05f;
    Player.Penetration = 1;
    Player.AttackSpeed = 1.5f;
    Player.bIsGuard = false;
}

void UGameManager::SpawnEnemy(ETypeEnemy EnemyType)
{
    //float minRadius;

    if (EnemyCount >= EnemyCapacity)
    {
        ResizeEnemyList();
    }

    UCharacterEnemy* NewEnemy = new UCharacterEnemy(EnemyType);

    NewEnemy->Location = GetEnemySpawnPosition();
    NewEnemy->CharacterType = ETypeCharacter::ETC_Enemy;

    EnemyList[EnemyCount] = NewEnemy;

    ++EnemyCount;

}
void UGameManager::ResizeItemList()
{
    int NewCapacity = ItemCapacity * 2;

    UItem** NewList =
        new UItem * [NewCapacity]();

    for (int i = 0; i < ItemCount; ++i)
    {
        NewList[i] = ItemList[i];
    }

    delete[] ItemList;

    ItemList = NewList;
    ItemCapacity = NewCapacity;
}

// 꽉차면 크기 2배로 확장
void UGameManager::ResizeEnemyList()
{
    int NewCapacity = EnemyCapacity * 2;

    UCharacterEnemy** NewList =
        new UCharacterEnemy * [NewCapacity]();

    for (int i = 0; i < EnemyCount; ++i)
    {
        NewList[i] = EnemyList[i];
    }

    delete[] EnemyList;

    EnemyList = NewList;
    EnemyCapacity = NewCapacity;
}

// 맨 마지막 적을 빈자리에 넣기
void UGameManager::RemoveEnemy(int Index)
{
    if (Index < 0 || Index >= EnemyCount)
    {
        return;
    }

    delete EnemyList[Index];

    EnemyList[Index] = EnemyList[EnemyCount - 1];

    EnemyList[EnemyCount - 1] = nullptr;

    --EnemyCount;
}

void UGameManager::ClearItem()
{
    for (int i = 0; i < ItemCount; ++i)
    {
        delete ItemList[i];
        ItemList[i] = nullptr;
    }

    ItemCount = 0;
}

void UGameManager::ClearEnemies()
{
    for (int i = 0; i < EnemyCount; ++i)
    {
        delete EnemyList[i];
        EnemyList[i] = nullptr;
    }

    EnemyCount = 0;
}

void UGameManager::ResizeProjectileList()
{
    int NewCapacity = ProjectileCapacity * 2;

    UProjectile** NewList =
        new UProjectile * [NewCapacity]();

    for (int i = 0; i < ProjectileCount; ++i)
    {
        NewList[i] = ProjectileList[i];
    }

    delete[] ProjectileList;

    ProjectileList = NewList;
    ProjectileCapacity = NewCapacity;
}

void UGameManager::SpawnProjectile(FVector Location, FVector Velocity, ETypeCharacter type, float Damage, int Cnt)
{
    UProjectile* NewProjectile = nullptr;

    if (ProjectileCount >= ProjectileCapacity)
    {
        ResizeProjectileList();
    }

    if (type == ETypeCharacter::ETC_EnemyProjectile)
    {
        NewProjectile = new UProjectileEnemy(Damage);
    }
    else if (type == ETypeCharacter::ETC_PlayerProjectile)
    {
        NewProjectile = new UProjectilePlayer(Damage, Cnt);
    }
    else
    {
        return;
    }

    if (NewProjectile == nullptr)
    {
        return;
    }

    NewProjectile->Location = Location;
    NewProjectile->Velocity = Velocity;

    ProjectileList[ProjectileCount] = NewProjectile;

    ++ProjectileCount;
}

void UGameManager::SpawnItem(FVector location) {
    if (ItemCount >= ItemCapacity)
    {
        ResizeItemList();
    }

    int roll = rand() % 100;
    UItem* NewItem;
    if (roll < 10) {
        NewItem = new UItemBomb();
    }
    else if (roll < 20) {
        NewItem = new UItemHeal();
    }
    else if (roll < 30) {
        NewItem = new UItemMagnet();
    }
    else {
        NewItem = new UItemEXP();
    }

    NewItem->Location = location;

    ItemList[ItemCount] = NewItem;

    ++ItemCount;
}

void UGameManager::RemoveItem(int Index) {
    if (Index < 0 || Index >= ItemCount)
    {
        return;
    }

    delete ItemList[Index];

    ItemList[Index] = ItemList[ItemCount - 1];

    ItemList[ItemCount - 1] = nullptr;

    --ItemCount;
}

void UGameManager::ClearProjectiles()
{
    for (int i = 0; i < ProjectileCount; ++i)
    {
        delete ProjectileList[i];
        ProjectileList[i] = nullptr;
    }

    ProjectileCount = 0;
}

void UGameManager::RemoveProjectile(int Index)
{
    if (Index < 0 || Index >= ProjectileCount)
    {
        return;
    }

    delete ProjectileList[Index];

    ProjectileList[Index] =
        ProjectileList[ProjectileCount - 1];

    ProjectileList[ProjectileCount - 1] = nullptr;

    --ProjectileCount;
}

void UGameManager::UpdateEnemySpawn(float DeltaTime)
{
    EnemySpawnTimer += DeltaTime;

    if (EnemySpawnTimer >= EnemySpawnInterval)
    {
        int RandomValue = rand() % 100;


        if (RandomValue < 40)
        {
            SpawnEnemy(ETypeEnemy::ETE_Walker);
        }
        else if (RandomValue < 80)
        {
            SpawnEnemy(ETypeEnemy::ETE_Runner);
        }
        else
        {
            SpawnEnemy(ETypeEnemy::ETE_Ranger);
        }

        EnemySpawnTimer = 0.0f;
    }
}

#pragma endregion


FVector UGameManager::GetEnemySpawnPosition()
{
    float RandomAngle =
        ((float)rand() / RAND_MAX) * 2.0f * 3.141592f;

    float MinRadius = EnemySpawnMinRadius;
    float MaxRadius = 1.0f;

    float RandomRadius =
        MinRadius +
        ((float)rand() / RAND_MAX) * (MaxRadius - MinRadius);

    float X =
        Player.Location.x +
        cosf(RandomAngle) * RandomRadius;

    float Y =
        Player.Location.y +
        sinf(RandomAngle) * RandomRadius;

    return FVector(X, Y, 0.0f);
}

void UGameManager::SetGameState(EGameState currentState)
{
    this->currentState = currentState;
}

void UGameManager::SetDifficulty(EGameDifficulty NewDifficulty)
{
    Difficulty = NewDifficulty;

    switch (Difficulty)
    {
    case EGameDifficulty::Easy:
        GameClearTime = 10.0f;
        // 쉬움모드에서 변경할 내용
        break;
    case EGameDifficulty::Hard:
        GameClearTime = 60.0f;
        break;
    }
}

void UGameManager::UpdateScore()
{
    float DifficultyMultiplier = 1.0f;

    switch (Difficulty)
    {
    case EGameDifficulty::Easy:
        DifficultyMultiplier = 1.0f;
        break;

    case EGameDifficulty::Hard:
        DifficultyMultiplier = 1.5f;
        break;
    }

    const float TimeScoreMultiplier = 10.0f;
    const float KillScoreMultiplier = 100.0f;

    Score = DifficultyMultiplier *(GameTime * TimeScoreMultiplier + KillCount * KillScoreMultiplier);
}

void UGameManager::UpdateGameTime(float DeltaTime)
{
    GameTime += DeltaTime;
    Delta = DeltaTime;
}

void UGameManager::CheckGameOver()
{
     if (Player.Hp <= 0.0f)
    {
        Lose();
    }
}

void UGameManager::CheckGameClear()
{
    if (GameTime >= GameClearTime)
    {
        Win();
    }
}
