#pragma once
#include "Math.h"
#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"
#include "ProjectileEnemy.h"
#include "ProjectilePlayer.h"


// 게임 현재 상태
enum EGameState
{
	Title,
	Playing,
	Paused,
	GameOver,
	GameClear,
};

enum class EGameDifficulty
{
	Easy,
	Hard
};

class UGameManager
{
public:
	UGameManager();
	~UGameManager();

	void Initialize();
	void Update(float DeltaTime);
	void ResetGame();

	void SpawnPlayer();
	void SpawnEnemy(ETypeCharacter EnemyType);
	void SpawnProjectile(FVector Location, FVector Velocity, ETypeCharacter type, float Damage);

	void RemoveEnemy(int Index);

	void PauseGame();
	void ResumeGame();

	void Lose();
	void Win();

	bool IsTitle() const;
	bool IsPlaying() const;
	bool IsPaused() const;

	UCharacterPlayer* GetPlayer()
	{
		return &Player;
	}

	UCharacterEnemy** GetEnemyList()
	{
		return EnemyList;
	}
	int GetEnemyListCount()
	{
		return EnemyCount;
	}
	UProjectile** GetProjectileList()
	{
		return ProjectileList;
	}
	int GetProjectileListCount()
	{
		return ProjectileCount;
	}

	EGameState GetGameState() const
	{
		return currentState;
	}

	void SetDifficulty(EGameDifficulty NewDifficulty);

	void ReturnToTitle();

	float GetGameTime() const
	{
		return GameTime;
	}

	float GetRemainingTime() const
	{
		float RemainingTime = GameClearTime - GameTime;

		if (RemainingTime < 0.0f)
		{
			RemainingTime = 0.0f;
		}

		return RemainingTime;
	}

	int GetScore() const
	{
		return Score;
	}


private:
	EGameState currentState = Title;
	EGameDifficulty Difficulty = EGameDifficulty::Easy;

	int Score = 0;

	float GameTime;
	float GameClearTime;

	float EnemySpawnTimer;
	float EnemySpawnInterval;

	FVector GetEnemySpawnPosition();
	float EnemySpawnMinRadius = 0.5f;

	UCharacterPlayer Player;

	UCharacterEnemy** EnemyList;

	int EnemyCount;
	int EnemyCapacity;

	void ResizeEnemyList();
	void ClearEnemies();
	void UpdateEnemySpawn(float DeltaTime);

	UProjectile** ProjectileList;
	int ProjectileCount;
	int ProjectileCapacity;

	
	//void UpdateProjectiles(float DeltaTime);
	void RemoveProjectile(int Index);
	void ClearProjectiles();
	void ResizeProjectileList();

	void UpdateGameTime(float DeltaTime);
	void CheckGameOver();
	void CheckGameClear();
};