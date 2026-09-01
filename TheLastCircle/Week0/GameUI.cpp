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

    ImGui::SetNextWindowPos(
        Center,
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );

    // 윈도우 사이즈 고정
    ImGui::SetNextWindowSize(
        ImVec2(320.0f, 280.0f),
        ImGuiCond_Always
    );

    switch (type)
    {
    case EPopupType::Title:
        break;
    case EPopupType::Pause:
        RenderPausePopup(GameManager);
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
    ImGuiViewport* Viewport = ImGui::GetMainViewport();
    ImVec2 Center = Viewport->GetCenter();

    ImGui::SetNextWindowSize(
        ImVec2(400.0f, 400.0f),
        ImGuiCond_Always
    );

    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("TitleWindow", nullptr, WindowFlags);

    const char* TitleText = "THE LAST CIRCLE";

    float TitleTextWidth =
        ImGui::CalcTextSize(TitleText).x;

    ImGui::SetCursorPosY(60.0f);

    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - TitleTextWidth) * 0.5f );

    ImGui::Text("%s", TitleText);


}

void UGameUI::RenderPausePopup(UGameManager* GameManager)
{
    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("PausePopup", nullptr, WindowFlags);

    const char* PauseText = "PAUSED";
    int tmpScore = 999;

    ImGui::Text("%s", PauseText);
    ImGui::Text("%d", tmpScore);

    if (ImGui::Button("Resume"))
    {
        GameManager->ResumeGame();
    }

    if (ImGui::Button("Restart"))
    {
        GameManager->ResetGame();
    }

    if (ImGui::Button("Home"))
    {
        GameManager->ReturnToTitle();
    }

    ImGui::End();
}

void UGameUI::RenderGameOverPopup(UGameManager* GameManager)
{
    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("GameOverPopup", nullptr, WindowFlags);

    const char* PauseText = "Game Over!!";
    int tmpScore = 000;

    ImGui::Text("%s", PauseText);
    ImGui::Text("%d", tmpScore);

    if (ImGui::Button("Restart"))
    {
        GameManager->ResetGame();
    }

    if (ImGui::Button("Home"))
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
        ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("GameClearPopup", nullptr, WindowFlags);

    const char* PauseText = "Game Clear!!";
    int tmpScore = 999;

    ImGui::Text("%s", PauseText);
    ImGui::Text("%d", tmpScore);

    if (ImGui::Button("Restart"))
    {
        GameManager->ResetGame();
    }

    if (ImGui::Button("Home"))
    {
        GameManager->ReturnToTitle();
    }

    ImGui::End();
}

#pragma endregion

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

    case GameOver:
        RenderPopup(GameManager, EPopupType::GameOver);
        break;

    case GameClear:
        RenderPopup(GameManager, EPopupType::GameClear);
        break;
    }
}

