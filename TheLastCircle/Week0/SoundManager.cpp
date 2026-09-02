#include "SoundManager.h"

void USoundManager::Initialize()
{
    DirectX::AUDIO_ENGINE_FLAGS Flags = DirectX::AudioEngine_Default;

    AudioEngine = std::make_unique<DirectX::AudioEngine>(Flags);

}

void USoundManager::Update()
{
    if (AudioEngine == nullptr)
    {
        return;
    }

    if (!AudioEngine->Update())
    {
        if (AudioEngine->IsCriticalError())
        {
            AudioEngine->Reset();
        }
    }
}

bool USoundManager::LoadSFX(ESFXType Type, const wchar_t* FilePath)
{
    if (AudioEngine == nullptr)
    {
        return false;
    }

    int Index = static_cast<int>(Type);

    if (Index < 0 || Index >= static_cast<int>(ESFXType::Count))
    {
        return false;
    }

    SFXSounds[Index] = std::make_unique<DirectX::SoundEffect>(AudioEngine.get(), FilePath);

    return true;
}

bool USoundManager::LoadBGM(const wchar_t* FilePath)
{
    if (AudioEngine == nullptr)
    {
        return false;
    }

    BGMSound = std::make_unique<DirectX::SoundEffect>(AudioEngine.get(), FilePath);

    BGMInstance = BGMSound->CreateInstance();

    BGMInstance->SetVolume(BGMVolume);

    return true;
}


# pragma region Volume

void USoundManager::SetMasterVolume(float Volume)
{
    if (Volume < 0.0f)
    {
        Volume = 0.0f;
    }

    if (Volume > 1.0f)
    {
        Volume = 1.0f;
    }

    MasterVolume = Volume;

    if (AudioEngine == nullptr)
    {
        return;
    }

    AudioEngine->SetMasterVolume(MasterVolume);
}

void USoundManager::SetSFXVolume(float Volume)
{
    // error 검출
    if (Volume < 0.0f)
    {
        Volume = 0.0f;
    }

    if (Volume > 1.0f)
    {
        Volume = 1.0f;
    }

    SFXVolume = Volume;
}

void USoundManager::SetBGMVolume(float Volume)
{
    if (Volume < 0.0f)
    {
        Volume = 0.0f;
    }

    if (Volume > 1.0f)
    {
        Volume = 1.0f;
    }

    BGMVolume = Volume;

    if (BGMInstance == nullptr)
    {
        return;
    }

    BGMInstance->SetVolume(BGMVolume);
}

#pragma endregion


void USoundManager::PlaySFX(ESFXType Type)
{
    int Index = static_cast<int>(Type);

    // 잘못된 Type 방지
    if (Index < 0 || Index >= static_cast<int>(ESFXType::Count))
    {
        return;
    }

    // 아직 로드하지 않은 사운드
    if (SFXSounds[Index] == nullptr)
    {
        return;
    }

    SFXSounds[Index]->Play(SFXVolume, 0.0f, 0.0f);
}

void USoundManager::PlayBGM()
{
    if (BGMInstance == nullptr)
    {
        return;
    }

    // loop = true
    BGMInstance->Play(true);
}

void USoundManager::StopBGM()
{
    if (BGMInstance == nullptr)
    {
        return;
    }

    BGMInstance->Stop();
}

void USoundManager::PauseBGM()
{
    if (BGMInstance == nullptr)
    {
        return;
    }

    BGMInstance->Pause();
}

void USoundManager::ResumeBGM()
{
    if (BGMInstance == nullptr)
    {
        return;
    }

    BGMInstance->Resume();
}

// 모든 사운드 관리
void USoundManager::PauseAllSound()
{
    if (AudioEngine == nullptr)
    {
        return;
    }

    AudioEngine->Suspend();
}

void USoundManager::ResumeAllSound()
{
    if (AudioEngine == nullptr)
    {
        return;
    }

    AudioEngine->Resume();
}