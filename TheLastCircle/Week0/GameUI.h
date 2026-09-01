#pragma once

#include "CharacterPlayer.h"

class UGameManager;

enum class EPopupType
{
    Title,
    Pause,
    Upgrade,
    GameOver,
    GameClear
};

class UGameUI
{
public:


    void UIRender(UGameManager* GameManager);

private:
    void RenderTitle(UGameManager* GameManager);
    void RenderDifficultyPopup(UGameManager* GameManager);
    void RenderCreditsPopup();

    void RenderPopup(UGameManager* GameManager, EPopupType type);
    void RenderPausePopup(UGameManager* GameManager);
    void RenderGameOverPopup(UGameManager* GameManager);
    void RenderGameClearPopup(UGameManager* GameManager);

    void RenderChoicePopup(UGameManager* GameManager, ETypeUpgrade* Choices);

};