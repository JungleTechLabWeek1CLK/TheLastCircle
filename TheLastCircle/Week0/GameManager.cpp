#include "GameManager.h"
#include <iostream>

GameManager::GameManager() 
    : currentState(Title), 
    GameTime(0.0f), 
    GameClearTime(60.0f), 
    EnemyList(nullptr),EnemyCount(0), 
    EnemyCapacity(30), EnemySpawnTimer(0.0f), 
    EnemySpawnInterval(100.0f)
{

}


GameManager::~GameManager()
{
    ClearEnemies();

    delete[] EnemyList;

    EnemyList = nullptr;
}

void GameManager::Initialize()
{
    currentState = Title;

    GameTime = 0.0f;
    EnemyCount = 0;
    EnemyCapacity = 30;

    EnemyList = new UCharacterEnemy * [EnemyCapacity]();

    // 60초 생존하면 게임 클리어
    GameClearTime = 60.0f;
}

void GameManager::Update(float DeltaTime)
{
    /*if (currentState != Playing)
    {
        return;
    }*/

    UpdateGameTime(DeltaTime);
    UpdateEnemySpawn(DeltaTime);

    CheckGameOver();
    CheckGameClear();
}


#pragma region GameStateFunction

void GameManager::ResetGame()
{
    GameTime = 0.0f;

    currentState = Playing;

    ClearEnemies();
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

void GameManager::SpawnEnemy()
{
    //float minRadius;

    if (EnemyCount >= EnemyCapacity)
    {
        ResizeEnemyList();
    }

    UCharacterEnemy* NewEnemy = new UCharacterEnemy();

    NewEnemy->Location = FVector(0.8f, 0.0f, 0.0f);
    NewEnemy->CharacterType = ETypeCharacter::ETC_Enemy;

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

void GameManager::UpdateEnemySpawn(float DeltaTime)
{
    EnemySpawnTimer += DeltaTime;

    if (EnemySpawnTimer >= EnemySpawnInterval)
    {
        SpawnEnemy();

        EnemySpawnTimer = 0.0f;
    }
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

#pragma endregion


void GameManager::UpdateGameTime(float DeltaTime)
{

}

void GameManager::CheckGameOver()
{

}

void GameManager::CheckGameClear()
{

}
