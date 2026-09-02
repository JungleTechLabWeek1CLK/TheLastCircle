#include "GameUI.h"
#include "GameManager.h"

#include "ImGui/imgui.h"

#include <Windows.h>
#include <string.h>


#pragma region Popups

void UGameUI::RenderTime(UGameManager* GameManager)
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoBackground;     
    window_flags |= ImGuiWindowFlags_NoTitleBar;    
    window_flags |= ImGuiWindowFlags_NoResize;       
    window_flags |= ImGuiWindowFlags_NoScrollbar;    
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize; 

    window_flags |= ImGuiWindowFlags_NoMove;

    auto Position = ImGui::GetMainViewport()->GetCenter();
    Position.y = 7.f;
    Position.x -= 130.f;
    ImGui::SetNextWindowPos(Position, ImGuiCond_Always);

    ImGui::Begin("Timer", nullptr, window_flags);

    ImGui::Text("Time: %.1f", GameManager->GetRemainingTime());

    ImGui::End();
}

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
            ImVec2(WindowSize.x*1.5f, WindowSize.y*1.5f),
            ImGuiCond_Always
        );
        RenderPausePopup(GameManager);
        break;
    case EPopupType::Upgrade:
        ImGui::SetNextWindowSize(
            ImVec2(
                ScreenSize.x * 0.9f,
                ScreenSize.y * 0.75f
            ),
            ImGuiCond_Always
        );
        RenderChoicePopup(GameManager,GameManager->GetPlayer()->Upgrades);
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
        ImGui::OpenPopup("CreditsPopup");
    }

    ImGui::Spacing();

    // Exit 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowSize.x - ButtonSize.x) * 0.5f
    );

    if (ImGui::Button("Exit", ButtonSize))
    {
        PostQuitMessage(0);
    }

    RenderDifficultyPopup(GameManager);
    RenderCreditsPopup();

    ImGui::End();
}

void UGameUI::RenderPausePopup(UGameManager* GameManager)
{
    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("PausePopup", nullptr, WindowFlags);

    float WindowWidth = ImGui::GetWindowWidth();
    float WindowHeight = ImGui::GetWindowHeight();

    const char* GameClearText = "Paused";

    ImVec2 ButtonSize(
        WindowWidth * 0.35f,
        WindowHeight * 0.1f
    );

    ImVec2 WindowSize(
        WindowWidth,
        WindowHeight
    );

    int Score = GameManager->GetScore();

    const char* ScoreText = "SCORE:999";

    float TextWidth = ImGui::CalcTextSize(GameClearText).x;
    float ScoreWidth = ImGui::CalcTextSize(ScoreText).x;

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.05f));

    ImGui::SetCursorPosX(
        (WindowWidth - TextWidth) * 0.5f
    );
    ImGui::Text("%s", GameClearText);

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.1f));

    ImGui::SetCursorPosX(
        (WindowWidth - ScoreWidth) * 0.5f
    );

    ImGui::Text("SCORE:%d", Score);

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.05f));

    int BGMVolume = USoundManager::GetInstance().GetBGMVolume()*100;
    int SFXVolume = USoundManager::GetInstance().GetSFXVolume() *100;

    float SliderWidth = WindowWidth * 0.5f;

    // BGM
    ImGui::SetCursorPosX((WindowWidth - SliderWidth) * 0.5f);

    ImGui::SetNextItemWidth(SliderWidth);

    if (ImGui::SliderInt(" BGM", &BGMVolume, 0.0f, 100.0f))
    {
        USoundManager::GetInstance().SetBGMVolume(float(BGMVolume / 100.0f));
    }

    ImGui::SetCursorPosX((WindowWidth - SliderWidth) * 0.5f);

    ImGui::SetNextItemWidth(SliderWidth);

    if (ImGui::SliderInt(" SFX", &SFXVolume, 0.0f, 100.0f))
    {
        USoundManager::GetInstance().SetSFXVolume(float(SFXVolume/100.0f));
    }

    ImGui::Dummy(ImVec2(0.f, WindowSize.y * 0.05f));
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

void UGameUI::RenderCreditsPopup()
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

    if (ImGui::BeginPopupModal("CreditsPopup", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        ImGui::SetWindowFontScale(1.0f);

        const char* Text = "Made By";
        const char* CreditNames[] = {"aaaaaaaaa", "bbbbbbbb", "ccccccccccc"};
       

        float TextWidth = ImGui::CalcTextSize(Text).x;

        ImGui::SetCursorPosX(
            (ImGui::GetWindowWidth() - TextWidth) * 0.5f
        );

        ImGui::Text("%s", Text);

        ImGui::Separator();

        ImGui::SetWindowFontScale(1.0f);

        ImGui::Dummy(
            ImVec2(0.0f, PopupSize.y * 0.1f)
        );

        int CreditCount = sizeof(CreditNames) / sizeof(CreditNames[0]);

        for (int i = 0; i < CreditCount; i++)
        {
            ImGui::PushID(i);

            float NameWidth = ImGui::CalcTextSize(CreditNames[i]).x;

            ImGui::SetCursorPosX(
                (ImGui::GetWindowWidth() - NameWidth) * 0.5f
            );

            ImGui::Text("%s", CreditNames[i]);

            ImGui::Dummy(
                ImVec2(0.0f, PopupSize.y * 0.05f)
            );

            if (i == CreditCount - 1)
            {
                ImGui::Dummy(
                    ImVec2(0.0f, PopupSize.y * 0.1f)
                );
            }

            ImGui::PopID();
        }
        

        ImVec2 SmallButtonSize(
            PopupSize.x * 0.2f,
            PopupSize.y * 0.1f
        );

        ImGui::SetWindowFontScale(1.0f);

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
            GameManager->SetDifficulty(EGameDifficulty::Hard);
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

    int Score = GameManager->GetScore();
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

    int Score = GameManager->GetScore();
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
    //int ChoiceCount = sizeof(Choices)/ sizeof(ETypeUpgrade);
    int ChoiceCount = 3;

    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("UpgradePopup", nullptr, WindowFlags);
    ImGui::SetWindowFontScale(1.0f);

    float WindowWidth = ImGui::GetWindowWidth();
    float WindowHeight = ImGui::GetWindowHeight();


    const char* Title = "SELECT UPGRADE";
    float TitleWidth = ImGui::CalcTextSize(Title).x;

    float SideMargin = WindowWidth * 0.01f;   // 좌우 여백 1%
    float CardSpacing = WindowWidth * 0.01f;  // 카드 사이 간격 1%

    float ButtonWidth =
        (WindowWidth
            - SideMargin * 2
            - CardSpacing * (ChoiceCount - 1))
        / ChoiceCount;

    float ButtonHeight = WindowHeight * 0.7f;

    // 가운데 정렬
    ImGui::SetCursorPosX(
        (WindowWidth - TitleWidth) * 0.5f
    );

    ImGui::Text("%s", Title);

    ImGui::Dummy(
        ImVec2(0.0f, WindowHeight * 0.05f)
    );

    // 여백 적용
    ImGui::SetCursorPosX(SideMargin);

    for (int i = 0; i < ChoiceCount; i++)
    {
        ImGui::PushID(i);
        const char* CardName = "";
        const char* Description = "";
        switch (Choices[i])
        {
        case ETypeUpgrade::ETU_Damage:
            CardName = "Damage+";
            Description = "Increase attack damage";
            break;
        case ETypeUpgrade::ETU_Hp:
            CardName = "HP+";
            Description = "Increase maximum HP";
            break;
        case ETypeUpgrade::ETU_Speed:
            CardName = "Speed+";
            Description = "Increase movement speed.";
            break;
        case ETypeUpgrade::ETU_bullets:
            CardName = "Bullets+";
            Description = "Increase number of bullets.";
            break;

        case ETypeUpgrade::ETU_Penetration:
            CardName = "Piercing+";
            Description = "Bullets pierce more enemies";
            break;
        case ETypeUpgrade::ETU_AttackSpeed:
            CardName = "DPS+";
            Description = "Increase attack Speed";
            break;
        case ETypeUpgrade::ETU_Scale:
            CardName = "Scale-";
            Description = "Become smaller";
            break;

        }

        ImGui::BeginChild("UpgradeCard",
            ImVec2(ButtonWidth, ButtonHeight), // 크기
            true // 테두리 표시
        );

        // 현재 창(Window) 또는 레이아웃 내에서 위젯을 추가로 배치할 수 있는 남은 가용 공간의 크기
        float InnerWidth = ImGui::GetContentRegionAvail().x;

        // 제목 중앙 정렬
        float NameWidth = ImGui::CalcTextSize(CardName).x;

        ImGui::SetCursorPosX((ButtonWidth - NameWidth) * 0.5f);

        ImGui::Text("%s", CardName);

        // 구분선
        ImGui::Separator();

        ImGui::Dummy(ImVec2(0.0f, ButtonHeight * 0.1f));

        ImGui::TextWrapped("%s", Description);

        float SelectHeight =
            ButtonHeight * 0.18f;

        ImGui::SetCursorPosY(ButtonHeight - SelectHeight - ButtonHeight * 0.08f);

        if (ImGui::Button("SELECT", ImVec2(InnerWidth, SelectHeight)))
        {
            switch (Choices[i])
            {
            case ETypeUpgrade::ETU_Damage:
                GameManager->GetPlayer()->Upgrade(ETypeUpgrade::ETU_Damage);
                break;
            case ETypeUpgrade::ETU_Hp:
                GameManager->GetPlayer()->Upgrade(ETypeUpgrade::ETU_Hp);
                break;
            case ETypeUpgrade::ETU_Speed:
                GameManager->GetPlayer()->Upgrade(ETypeUpgrade::ETU_Speed);
                break;
            case ETypeUpgrade::ETU_bullets:
                GameManager->GetPlayer()->Upgrade(ETypeUpgrade::ETU_bullets);
                break;
            }
            GameManager->ResumeGame();
            
        }
        // 게임 진행
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::PopID();
    }
   
    
    ImGui::End();
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
        RenderTime(GameManager);
        break;

    case Paused:
        RenderPopup(GameManager, EPopupType::Pause);
        break;

    case Upgrade:
        RenderPopup(GameManager, EPopupType::Upgrade);
        break;

    case GameOver:
        RenderPopup(GameManager, EPopupType::GameOver);
        break;

    case GameClear:
        RenderPopup(GameManager, EPopupType::GameClear);
        break;
    }
}

