#pragma once

#include <Windows.h>
#include <xaudio2.h>

#pragma comment(lib, "xaudio2.lib")

class UWavManager
{
public:
    UWavManager();
    ~UWavManager();

    bool Initialize();

    bool LoadSound(const wchar_t* FilePath);

    bool Play() {};

    void Release() {};

private:

    // XAudio2 전체 엔진 => 필수 
    IXAudio2* AudioEngine = nullptr;

    // 최종 출력 장치
    IXAudio2MasteringVoice* MasteringVoice = nullptr;

    // 오디오 음원이 들어갈 곳
    IXAudio2SourceVoice* SourceVoice = nullptr;

    // CreateSourceVoice가 WAVEFORMATEX 요구
    WAVEFORMATEX WaveFormat = {};

    unsigned char* AudioData = nullptr;

    unsigned int AudioDataSize = 0;


};