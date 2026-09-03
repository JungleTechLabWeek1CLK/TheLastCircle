#pragma once
#include "Math.h"
#include "CharacterPlayer.h"
#include "CharacterEnemy.h"
#include "Projectile.h"
#include "ProjectileEnemy.h"
#include "ProjectilePlayer.h"
#include "ProjectileAxe.h"
#include "ItemEXP.h"
#include "ItemBomb.h"
#include "ItemHeal.h"
#include "ItemMagnet.h"
#include "SoundManager.h"


// 게임 현재 상태
enum EGameState
{
	Title,
	Playing,
	Paused,
	Upgrade,
	GameOver,
	GameClear,
};

enum class EGameDifficulty
{
	Easy,
	Hard
};

class URenderer;
class UGameManager
{
public:
	UGameManager();
	~UGameManager();

	void Initialize(URenderer*);
	void Update(float DeltaTime);
	void ResetGame();

	void SpawnPlayer();
	void SpawnEnemy(ETypeEnemy EnemyType);
	void SpawnProjectile(FVector Location, FVector Velocity, ETypeCharacter type, ETypeProjectile projectileType, float Damage, int Cnt);
	void SpawnItem(FVector location);

	void RemoveEnemy(int Index);
	void RemoveItem(int Index);

	void PauseGame();
	void ResumeGame();

	void Lose();
	void Win();

	bool IsTitle() const;
	bool IsPlaying() const;
	bool IsPaused() const;
	bool IsUpgrade() const;

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

	UItem** GetItemList()
	{
		return ItemList;
	}
	int GetItemListCount()
	{
		return ItemCount;
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

	void SetGameState(EGameState currentState);


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

	float GetScore() const
	{
		return Score;
	}

	void SetScore(float score)
	{
		this->Score = score;
	}
	void ResetScore()
	{
		this->Score = 0;
	}

	void AddKill();

	float GetDeltaTime() {
		return Delta;
	}

private:
	EGameState currentState = Title;
	EGameDifficulty Difficulty = EGameDifficulty::Easy;

	float Score = 0.f;
	int KillCount = 0;

	float GameTime;
	float GameClearTime;
	float Delta;

	float EnemySpawnTimer;
	float EnemySpawnInterval;

	FVector GetEnemySpawnPosition();
	float EnemySpawnMinRadius = 0.5f;

	UCharacterPlayer Player;

	UCharacterEnemy** EnemyList;

	int EnemyCount;
	int EnemyCapacity;

	UItem** ItemList;

	int ItemCount;
	int ItemCapacity;

	void ResizeItemList();
	void ClearItem();

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

	URenderer* Renderer;

	void UpdateScore();
	void UpdateGameTime(float DeltaTime);
	void CheckGameOver();
	void CheckGameClear();

	
};