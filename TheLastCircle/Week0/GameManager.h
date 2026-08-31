#pragma once
#include "Math.h"
#include "CharacterEnemy.h"

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
	void SpawnEnemy();


	void PauseGame();
	void ResumeGame();

	void Lose();
	void Win();

	bool IsPlaying() const;
	bool IsPaused() const;


private:
	EGameState currentState = Title;
	float GameTime;
	float GameClearTime;

	float EnemySpawnTimer;
	float EnemySpawnInterval;

	UCharacterEnemy Player;

	UCharacterEnemy** EnemyList;

	int EnemyCount;
	int EnemyCapacity;

	void ResizeEnemyList();
	void RemoveEnemy(int Index);
	void ClearEnemies();


	void UpdateEnemySpawn(float DeltaTime);

	void UpdateGameTime(float DeltaTime);
	void CheckGameOver();
	void CheckGameClear();


};