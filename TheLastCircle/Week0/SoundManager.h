#pragma once

#include <Audio.h>

enum class ESFXType
{
    PlayerShoot,
    PlayerHit,
    PlayerDie,
    EnemyHit,
    EnemyDie,
    LevelUp,
    GameOver,
    GameClear,

    Count
};

class USoundManager
{
public:

    // 싱글톤 설정
    static USoundManager& GetInstance()
    {
        static USoundManager Instance;
        return Instance;
    }

    // 복사 금지
    USoundManager(const USoundManager&) = delete;
    USoundManager& operator=(const USoundManager&) = delete;


    void Initialize();

    void Update();

    bool LoadSFX(ESFXType Type, const wchar_t* FilePath);
    bool LoadBGM(const wchar_t* FilePath);

    // no loop
    void PlaySFX(ESFXType Type);

    // loop
    void PlayBGM();
    void StopBGM();

    void PauseBGM();
    void ResumeBGM();

    void PauseAllSound();
    void ResumeAllSound();

    float GetMasterVolume() const
    {
        return MasterVolume;
    }

    float GetBGMVolume() const
    {
        return BGMVolume;
    }

    float GetSFXVolume() const
    {
        return SFXVolume;
    }

    // volume
    void SetMasterVolume(float Volume);
    void SetSFXVolume(float Volume);
    void SetBGMVolume(float Volume);

private:

    // 소유권이 명확; 공식에서 스마트 포인터 권장
    std::unique_ptr<DirectX::AudioEngine> AudioEngine;

    std::unique_ptr<DirectX::SoundEffect> SFXSounds[static_cast<int>(ESFXType::Count)];

    std::unique_ptr<DirectX::SoundEffect> BGMSound;

    std::unique_ptr<DirectX::SoundEffectInstance> BGMInstance;

    float MasterVolume = 1.0f;
    float SFXVolume = 0.5f;
    float BGMVolume = 0.5f;

    // 외부에서 객체 생성 불가능
    USoundManager() = default;
    ~USoundManager() = default;
};