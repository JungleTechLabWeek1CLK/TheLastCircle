#include "GameManager.h"
#include <cstdlib>
#include <cmath>


#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"



GameManager::GameManager() 
    : currentState(Title), 
    GameTime(0.0f), 
    GameClearTime(60.0f), 
    EnemyList(nullptr),EnemyCount(0), 
    EnemyCapacity(30), EnemySpawnTimer(0.0f), 
    EnemySpawnInterval(1.0f),
    ProjectileList(nullptr),
    ProjectileCount(0),
    ProjectileCapacity(30)
{

}


GameManager::~GameManager()
{
    ClearEnemies();
    ClearProjectiles();

    delete[] EnemyList;
    EnemyList = nullptr;

    delete[] ProjectileList;
    ProjectileList = nullptr;
}

void GameManager::Initialize()
{
    currentState = Title;

    GameTime = 0.0f;
    EnemyCount = 0;
    EnemyCapacity = 30;
    EnemyList = new UCharacterEnemy * [EnemyCapacity]();

    ProjectileCount = 0;
    ProjectileCapacity = 30;
    ProjectileList = new UProjectile * [ProjectileCapacity]();


    // 60초 생존하면 게임 클리어
    GameClearTime = 60.0f;
}

void GameManager::Update(float DeltaTime)
{
    if (currentState != Playing)
    {
        return;
    }

    UpdateGameTime(DeltaTime);
    UpdateEnemySpawn(DeltaTime);
    //UpdateProjectiles(DeltaTime);

    CheckGameOver();
    CheckGameClear();
}


#pragma region GameStateFunction

void GameManager::ResetGame()
{
    GameTime = 0.0f;
    EnemySpawnTimer = 0.0f;

    currentState = Playing;

    ClearEnemies();
    ClearProjectiles();
    SpawnPlayer();

}

void GameManager::PauseGame()
{
    if (currentState == Playing)
    {
        currentState = Paused;
    }
}

void GameManager::ResumeGame()
{
    if (currentState == Paused)
    {
        currentState = Playing;
    }
}

void GameManager::Lose()
{
    if (currentState != Playing)
    {
        return;
    }

    currentState = GameOver;
}

void GameManager::Win()
{
    if (currentState != Playing)
    {
        return;
    }

    currentState = GameClear;
}

#pragma endregion

bool GameManager::IsPlaying() const
{
    return (currentState == Playing) ? true : false;
}

bool GameManager::IsPaused() const
{
    return (currentState == Paused) ? true : false;
}

#pragma region SpawnFunction

void GameManager::SpawnPlayer()
{
    FVector SpawnPosition(0.0f, 0.0f, 0.0f);

    Player.Location = SpawnPosition;
}

void GameManager::SpawnEnemy(ETypeCharacter EnemyType)
{
    //float minRadius;

    if (EnemyCount >= EnemyCapacity)
    {
        ResizeEnemyList();
    }

    UCharacterEnemy* NewEnemy = new UCharacterEnemy();

    NewEnemy->Location = GetEnemySpawnPosition();
    NewEnemy->CharacterType = EnemyType;

    EnemyList[EnemyCount] = NewEnemy;

    ++EnemyCount;

}

// 꽉차면 크기 2배로 확장
void GameManager::ResizeEnemyList()
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
void GameManager::RemoveEnemy(int Index)
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

void GameManager::ClearEnemies()
{
    for (int i = 0; i < EnemyCount; ++i)
    {
        delete EnemyList[i];
        EnemyList[i] = nullptr;
    }

    EnemyCount = 0;
}

void GameManager::SpawnProjectile(FVector Location, FVector Velocity)
{
    if (ProjectileCount >= ProjectileCapacity)
    {
        return;
    }

    UProjectile* NewProjectile = new UProjectile(Location, Velocity);

    ProjectileList[ProjectileCount] = NewProjectile;

    ++ProjectileCount;
}

void GameManager::ClearProjectiles()
{
    for (int i = 0; i < ProjectileCount; ++i)
    {
        delete ProjectileList[i];
        ProjectileList[i] = nullptr;
    }

    ProjectileCount = 0;
}

void GameManager::RemoveProjectile(int Index)
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

void GameManager::UpdateEnemySpawn(float DeltaTime)
{
    EnemySpawnTimer += DeltaTime;

    if (EnemySpawnTimer >= EnemySpawnInterval)
    {
        int RandomValue = rand() % 100;

        if (RandomValue < 80)
        {
            SpawnEnemy(ETypeCharacter::ETC_Enemy);
        }
        else
        {
            SpawnEnemy(ETypeCharacter::ETC_EnemyProjectile);
        }

        EnemySpawnTimer = 0.0f;
    }
}

#pragma endregion


FVector GameManager::GetEnemySpawnPosition()
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

void GameManager::UpdateGameTime(float DeltaTime)
{
    GameTime += DeltaTime;
}

void GameManager::CheckGameOver()
{

}

void GameManager::CheckGameClear()
{

}
