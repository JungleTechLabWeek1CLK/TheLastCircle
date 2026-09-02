#include "WavManager.h"

#include <cstdio>
#include <cstring>
#include <assert.h>

#pragma comment(lib, "xaudio2.lib")

UWavManager::~UWavManager()
{
    Release();
}

bool UWavManager::Initialize()
{
    //  C++/WinRT 면 이미 COM이 initialize된다. 
    HRESULT hr = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) return false;

    // 생성
    HRESULT Result = XAudio2Create(
        &AudioEngine,
        0,
        XAUDIO2_DEFAULT_PROCESSOR
    );

    // 실패하면
    if (FAILED(Result))
    {
        assert(false && "잘못된 Xaduio2 생성입니다!\n");
        return false;
    }

    Result = AudioEngine->CreateMasteringVoice(&MasteringVoice);

    //실패시
    if (FAILED(Result))
    {
        assert(false && "잘못된 CreateMasteringVoice 입니다!\n");
        AudioEngine->Release();
        AudioEngine = nullptr;

        return false;
    }

    return true;
}

bool UWavManager::LoadSound(const wchar_t* filepath)
{
    FILE* File = nullptr;
    if (AudioEngine == nullptr)
    {
        return false;
    }

    // 바이너리로 읽는다.
    if (_wfopen_s(&File, filepath, L"rb") != 0)
    {
        return false;
    }


    // RIFF 읽기
    // WAV 파일 맨 앞에는 RIFF라는 문자가 존재(식별자)
    // Wav파일에 대해서는 
    // https://wowon.tistory.com/369 
    // http://soundfile.sapp.org/doc/WaveFormat/ 참고
    // 

    char RIFF[4];

    fread(RIFF, 1, 4, File);


    if (memcmp(RIFF, "RIFF", 4) != 0)
    {
        fclose(File);
        return false;
    }


    // RIFF 전체 크기(파일 크기 정보가 4byte)
    unsigned int RIFFSize = 0;

    fread(&RIFFSize, 4, 1, File);


    // Wave 체크

    char WAVE[4];

    fread(WAVE, 1, 4, File);

    if (memcmp(WAVE, "WAVE", 4) != 0)
    {
        fclose(File);
        return false;
    }

    // fmt(포멧)와 data를 찾았는지 기억
    bool FoundFormat = false;
    bool FoundData = false;

    while (!FoundFormat || !FoundData)
    {
        char ChunkID[4];

        unsigned int ChunkSize = 0;


        // Chunk  4바이트 읽기
        if (fread(ChunkID, 1, 4, File) != 4)
        {
            break;
        }


        // Chunk 크기 읽기
        if (fread(&ChunkSize, 4, 1, File) != 1)
        {
            break;
        }

        if (memcmp(ChunkID, "fmt ", 4) == 0)
        {


            FoundFormat = true;
        }
    }


    // data의 경우

    // 1. Subchunk2ID(4byte)
    // 2. Subchunk2Size(4byte)
    // => Duration = Subchunk2Size / ByteRate
    // 3. data(n byte)

}