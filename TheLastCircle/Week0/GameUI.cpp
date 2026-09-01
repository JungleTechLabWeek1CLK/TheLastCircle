#include "GameUI.h"
#include "GameManager.h"

#include "ImGui/imgui.h"

#include <Windows.h>


#pragma region Popups

void UGameUI::RenderPopup(UGameManager* GameManager, EPopupType type)
{
    ImGuiViewport* Viewport = ImGui::GetMainViewport();

    // 현재 화면의 중앙
    ImVec2 Center = Viewport->GetCenter();

    ImVec2 ScreenSize = Viewport->Size;

    // 화면 크기의 비율로 UI 크기 결정
    ImVec2 WindowSize(
        ScreenSize.x * 0.4f,   // 화면 가로의 40%
        ScreenSize.y * 0.5f    // 화면 세로의 50%
    );

    // 화면 중앙 배치
    ImGui::SetNextWindowPos(
        Center,
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );

    ImGui::SetNextWindowSize(
        WindowSize,
        ImGuiCond_Always
    );

    switch (type)
    {
    case EPopupType::Title:
        break;
    case EPopupType::Pause:
        ImGui::SetNextWindowSize(
            ImVec2(WindowSize.x, WindowSize.y*1.2f),
            ImGuiCond_Always
        );
        RenderPausePopup(GameManager);
        break;
    case EPopupType::Upgrade:
        RenderChoicePopup(GameManager, nullptr);  // TODO: fix this
        break;
    case EPopupType::GameOver:
        RenderGameOverPopup(GameManager);
        break;
    case EPopupType::GameClear:
        RenderGameClearPopup(GameManager);
        break;
    default:
        break;
    }

   
}

void UGameUI::RenderTitle(UGameManager* GameManager)
{
    // 폰트 스케일 테스트
    ImGui::GetIO().FontGlobalScale = 4.0f;
    ImGuiViewport* Viewport = ImGui::GetMainViewport();
    ImVec2 Center = Viewport->GetCenter();
    ImVec2 ScreenSize = Viewport->Size;

    // 화면 크기의 비율로 UI 크기 결정
    ImVec2 WindowSize(
        ScreenSize.x * 1.0f,   // 화면 가로의 40%
        ScreenSize.y * 1.0f    // 화면 세로의 50%
    );

    // 화면 중앙 배치
    ImGui::SetNextWindowPos(
        Center,
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );

    ImGui::SetNextWindowSize(WindowSize,ImGuiCond_Always);

    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("TitleWindow", nullptr, WindowFlags);

    const char* TitleText = "THE LAST CIRCLE";

    ImVec2 TitleTextSize = ImGui::CalcTextSize(TitleText);

    float TitleTextWidth = ImGui::CalcTextSize(TitleText).x;

    // 가로 가운데
    float TitleX =
        (ImGui::GetWindowWidth() - TitleTextSize.x) * 0.5f;

    // 세로 가운데
    float TitleY =
        (ImGui::GetWindowHeight() - TitleTextSize.y) * 0.15f;

    ImGui::SetCursorPos(
        ImVec2(TitleX, TitleY)
    );

    ImGui::Text("%s", TitleText);

    // 버튼 폰트 스케일
    ImGui::SetWindowFontScale(2.0f);

    ImGui::Spacing();
    // Dummy로 여백 만들기
    ImGui::Dummy(ImVec2(0.f, ScreenSize.y*0.15f));

    // 버튼 크기도 비율로
    ImVec2 ButtonSize(
        WindowSize.x * 0.5f,
        WindowSize.y * 0.12f
    );

    // Start 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowSize.x - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Start", ButtonSize))
    {
        ImGui::OpenPopup("DifficultyPopup");
    }

    ImGui::Spacing();

    // Credit 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowSize.x - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Credit", ButtonSize))
    {

    }

    ImGui::Spacing();

    // Exit 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowSize.x - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Exit", ButtonSize))
    {

    }

    RenderDifficultyPopup(GameManager);

    ImGui::End();
}

void UGameUI::RenderPausePopup(UGameManager* GameManager)
{
    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    float WindowWidth = ImGui::GetWindowWidth();
    float WindowHeight = ImGui::GetWindowHeight();

    ImGui::Begin("PausePopup", nullptr, WindowFlags);

    const char* GameClearText = "Paused";

    ImVec2 ButtonSize(
        WindowWidth * 0.7f,
        WindowHeight * 0.2f
    );

    ImVec2 WindowSize(
        WindowWidth,
        WindowHeight
    );

    int Score = 111;
    const char* ScoreText = "SCORE:999";

    float TextWidth = ImGui::CalcTextSize(GameClearText).x;
    float ScoreWidth = ImGui::CalcTextSize(ScoreText).x;

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));

    ImGui::SetCursorPosX(
        (WindowWidth - TextWidth) * 0.5f
    );
    ImGui::Text("%s", GameClearText);

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));

    ImGui::SetCursorPosX(
        (WindowWidth - ScoreWidth) * 0.5f
    );

    ImGui::Text("SCORE:%d", Score);

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));

    // 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowWidth - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Resume", ButtonSize))
    {
        GameManager->ResumeGame();
    }

    // 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowWidth - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Restart", ButtonSize))
    {
        GameManager->ResetGame();
    }

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.05f));

    // 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowWidth - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Home", ButtonSize))
    {
        GameManager->ReturnToTitle();
    }

    ImGui::End();
}

void UGameUI::RenderDifficultyPopup(UGameManager* GameManager)
{
    ImGuiViewport* Viewport = ImGui::GetMainViewport();
    ImVec2 Center = Viewport->GetCenter();

    ImVec2 ScreenSize = Viewport->Size;

    ImVec2 PopupSize(
        ScreenSize.x * 0.5f,
        ScreenSize.y * 0.5f
    );

    // 팝업 위치를 화면 중앙
    ImGui::SetNextWindowPos(Center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    ImGui::SetNextWindowSize(ImVec2(PopupSize), ImGuiCond_Always);

    ImGui::SetWindowFontScale(2.0f);

    if (ImGui::BeginPopupModal("DifficultyPopup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::SetWindowFontScale(1.0f);

        const char* Text = "SELECT DIFFICULTY";

        float TextWidth = ImGui::CalcTextSize(Text).x;

        ImGui::SetCursorPosX(
            (ImGui::GetWindowWidth() - TextWidth) * 0.5f
        );

        ImGui::Text("%s", Text);

        ImGui::SetWindowFontScale(1.5f);

        ImGui::Dummy(
            ImVec2(0.0f, PopupSize.y * 0.1f)
        );

        ImVec2 ButtonSize(
            PopupSize.x * 0.3f,
            PopupSize.y * 0.15f
        );

        ImVec2 SmallButtonSize(
            PopupSize.x * 0.2f,
            PopupSize.y * 0.1f
        );

        ImGui::SetWindowFontScale(1.0f);
        
        // 가운데 정렬
        ImGui::SetCursorPosX(
            (PopupSize.x - ButtonSize.x) * 0.5f
        );
        if (ImGui::Button("EASY", ButtonSize))
        {
            GameManager->SetDifficulty(EGameDifficulty::Easy);
            GameManager->ResetGame();

            ImGui::CloseCurrentPopup();
        }

        ImGui::Dummy(
            ImVec2(0.0f, PopupSize.y * 0.05f)
        );
        //ImGui::SameLine();

        // HARD
        ImGui::SetCursorPosX(
            (PopupSize.x - ButtonSize.x) * 0.5f
        );
        if (ImGui::Button("HARD", ButtonSize))
        {
            GameManager->SetDifficulty(EGameDifficulty::Easy);
            GameManager->ResetGame();

            ImGui::CloseCurrentPopup();
        }

        ImGui::Dummy(
            ImVec2(0.0f, PopupSize.y * 0.2f)
        );
        ImGui::Spacing();

        ImGui::SetWindowFontScale(0.7f);

        ImGui::SetCursorPosX(
            (PopupSize.x - SmallButtonSize.x) * 0.5f
        );
        if (ImGui::Button("Back", SmallButtonSize))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

}

void UGameUI::RenderGameOverPopup(UGameManager* GameManager)
{
    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    float WindowWidth = ImGui::GetWindowWidth();
    float WindowHeight = ImGui::GetWindowHeight();

    ImGui::Begin("GameOverPopup", nullptr, WindowFlags);

    const char* GameOverText = "Game Over";

    ImVec2 ButtonSize(
        WindowWidth * 0.7f,
        WindowHeight * 0.2f
    );

    ImVec2 WindowSize(
        WindowWidth,
        WindowHeight
    );

    int Score = 111;
    const char* ScoreText = "SCORE:999";

    float TextWidth = ImGui::CalcTextSize(GameOverText).x;
    float ScoreWidth = ImGui::CalcTextSize(ScoreText).x;

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));

    ImGui::SetCursorPosX(
        (WindowWidth - TextWidth) * 0.5f
    );
    ImGui::Text("%s", GameOverText);

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));

    ImGui::SetCursorPosX(
        (WindowWidth - ScoreWidth) * 0.5f
    );

    ImGui::Text("SCORE:%d", Score);

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));

    // 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowWidth - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Restart", ButtonSize))
    {
        GameManager->ResetGame();
    }

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.05f));

    // 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowWidth - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Home", ButtonSize))
    {
        GameManager->ReturnToTitle();
    }

    ImGui::End();
}

void UGameUI::RenderGameClearPopup(UGameManager* GameManager)
{
    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoTitleBar;

    float WindowWidth = ImGui::GetWindowWidth();
    float WindowHeight = ImGui::GetWindowHeight();

    ImGui::Begin("GameClearPopup", nullptr, WindowFlags);

    const char* GameClearText = "Game Clear";

    ImVec2 ButtonSize(
        WindowWidth * 0.7f,
        WindowHeight * 0.2f
    );

    ImVec2 WindowSize(
        WindowWidth,
        WindowHeight
    );

    int Score = 111;
    const char* ScoreText = "SCORE:999";

    float TextWidth = ImGui::CalcTextSize(GameClearText).x;
    float ScoreWidth = ImGui::CalcTextSize(ScoreText).x;

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));

    ImGui::SetCursorPosX(
        (WindowWidth - TextWidth) * 0.5f
    );
    ImGui::Text("%s", GameClearText);

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));

    ImGui::SetCursorPosX(
        (WindowWidth - ScoreWidth) * 0.5f
    );

    ImGui::Text("SCORE:%d", Score);

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));
    // 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowWidth - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Restart", ButtonSize))
    {
        GameManager->ResetGame();
    }

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.05f));

    // 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowWidth - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Home", ButtonSize))
    {
        GameManager->ReturnToTitle();
    }

    ImGui::End();
}

#pragma endregion

void UGameUI::RenderChoicePopup(UGameManager* GameManager, ETypeUpgrade* Choices)
{
    ImGuiViewport* Viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    float WindowWidth = ImGui::GetWindowWidth();
    float WindowHeight = ImGui::GetWindowHeight();

    ImGui::Begin("UpgradePopup", nullptr, WindowFlags);
    const char* cardName = "";

    for (int i = 0; i < 3; i++)
    {
        switch (Choices[i])
        {
        case ETypeUpgrade::ETU_Damage:
            cardName = "Damage+";
            break;
        case ETypeUpgrade::ETU_Hp:
            cardName = "HP+";
            break;
        case ETypeUpgrade::ETU_Speed:
            cardName = "Speed+";
             break;
        case ETypeUpgrade::ETU_bullets:
            cardName = "Bullets+";
            break;
        }

        ImGui::Button(cardName);

    }


}


void UGameUI::UIRender(UGameManager* GameManager)
{
    if (GameManager == nullptr)
    {
        return;
    }

    switch (GameManager->GetGameState())
    {
    case Title:
        RenderTitle(GameManager);
        break;

    case Playing:
        // 인게임 UI
        break;

    case Paused:
        RenderPopup(GameManager, EPopupType::Pause);
        break;

    case Upgrade:
        RenderChoicePopup(GameManager, GameManager->GetPlayer()->Upgrades);
        break;

    case GameOver:
        RenderPopup(GameManager, EPopupType::GameOver);
        break;

    case GameClear:
        RenderPopup(GameManager, EPopupType::GameClear);
        break;
    }
}

