#include "GameManager.h"
#include <cstdlib>
#include <cmath>


#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"



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
    EXPList(nullptr), EXPCount(0),
    EXPCapacity(30)
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

void UGameManager::Initialize()
{
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

    EXPCount = 0;
    EXPCapacity = 30;
    EXPList = new UItemEXP * [EXPCapacity]();
    // 60초 생존하면 게임 클리어
}

void UGameManager::Update(float DeltaTime)
{
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
    ClearEXP();
    SpawnPlayer();

}

void UGameManager::PauseGame()
{
    if (currentState == Playing)
    {
        currentState = Paused;
    }
}

void UGameManager::ResumeGame()
{
    if (currentState == Paused)
    {
        currentState = Playing;
    }
    else if (currentState == Upgrade)
    {
        currentState = Playing;
    }
}

void UGameManager::Lose()
{
    if (currentState != Playing)
    {
        return;
    }

    currentState = GameOver;
}

void UGameManager::Win()
{
    if (currentState != Playing)
    {
        return;
    }

    currentState = GameClear;
}

void UGameManager::ReturnToTitle()
{
    ClearEnemies();
    ClearProjectiles();

    GameTime = 0.0f;
    EnemySpawnTimer = 0.0f;
    Score = 0;

    currentState = Title;
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
    Player.Speed = 0.5f;
    Player.Damage = 100.f;
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
void UGameManager::ResizeEXPList()
{
    int NewCapacity = EXPCapacity * 2;

    UItemEXP** NewList =
        new UItemEXP * [NewCapacity]();

    for (int i = 0; i < EXPCount; ++i)
    {
        NewList[i] = EXPList[i];
    }

    delete[] EXPList;

    EXPList = NewList;
    EXPCapacity = NewCapacity;
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

void UGameManager::ClearEXP()
{
    for (int i = 0; i < EXPCount; ++i)
    {
        delete EXPList[i];
        EXPList[i] = nullptr;
    }

    EXPCount = 0;
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

void UGameManager::SpawnProjectile(FVector Location, FVector Velocity, ETypeCharacter type, float Damage)
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
        NewProjectile = new UProjectilePlayer(Damage);
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

void UGameManager::SpawnEXP(FVector location, float reword) {
    if (EXPCount >= EXPCapacity)
    {
        ResizeEXPList();
    }

    UItemEXP* NewEXP = new UItemEXP(reword);

    NewEXP->Location = location;

    EXPList[EXPCount] = NewEXP;

    ++EXPCount;
}

void UGameManager::RemoveEXP(int Index) {
    if (Index < 0 || Index >= EXPCount)
    {
        return;
    }

    delete EXPList[Index];

    EXPList[Index] = EXPList[EXPCount - 1];

    EXPList[EXPCount - 1] = nullptr;

    --EXPCount;
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
