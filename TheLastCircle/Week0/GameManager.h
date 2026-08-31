#pragma once
#include "Math.h"
#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"


// 게임 현재 상태
enum EGameState
{
	Title,
	Playing,
	Paused,
	GameOver,
	GameClear,
};

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Initialize();
	void Update(float DeltaTime);
	void ResetGame();

	void SpawnPlayer();
	void SpawnEnemy(ETypeCharacter EnemyType);
	void SpawnProjectile();

	void PauseGame();
	void ResumeGame();

	void Lose();
	void Win();

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
	UProjectile** GetPlayerProjectileList()
	{
		return ProjectileList;
	}
	int GetPlayerProjectileListCount()
	{
		return ProjectileCount;
	}


private:
	EGameState currentState = Title;
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
	void RemoveEnemy(int Index);
	void ClearEnemies();
	void UpdateEnemySpawn(float DeltaTime);

	UProjectile** ProjectileList;
	int ProjectileCount;
	int ProjectileCapacity;

	void UpdateProjectiles(float DeltaTime);
	void RemoveProjectile(int Index);
	void ClearProjectiles();

	void UpdateGameTime(float DeltaTime);
	void CheckGameOver();
	void CheckGameClear();




};