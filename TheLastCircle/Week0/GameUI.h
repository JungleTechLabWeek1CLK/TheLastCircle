#pragma once

class UGameManager;

enum class EPopupType
{
    Title,
    Pause,
    GameOver,
    GameClear
};

class UGameUI
{
public:


    void UIRender(UGameManager* GameManager);

private:
    void RenderPopup(UGameManager* GameManager, EPopupType type);
    void RenderPausePopup(UGameManager* GameManager);
    void RenderGameOverPopup(UGameManager* GameManager);
    void RenderGameClearPopup(UGameManager* GameManager);

};