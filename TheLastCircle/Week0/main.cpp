///////////////////////////////////////////////
// Headers

// ImgGui
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"



#include "Math.h"
#include "Shapes.h"
#include "Renderer.h"
#include "Physics.h"
#include "Character.h"
#include "GameManager.h"
#include "GameUI.h"
#include "SoundManager.h"

///////////////////////////////////////////////



///////////////////////////////////////////////
// declaration for using ImGui's handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    {
        return true;
    }


    switch (message)
    {
    case WM_DESTROY:
        // Signal that the app should quit
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
///////////////////////////////////////////////






///////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WCHAR WindowClass[] = L"JungleWindowClass";
    WCHAR Title[] = L"Game Tech Lab";
    WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };
    RegisterClassW(&wndclass);

    HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        300, 0, 1024, 1024,
        nullptr, nullptr, hInstance, nullptr);


    // Initializing URenderer Object
    URenderer Renderer;
    Renderer.Create(hWnd);


    // Initializing ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init((void*)hWnd);
    ImGui_ImplDX11_Init(Renderer.Device, Renderer.DeviceContext);


    // Necessary Variables for the main loop
    bool bIsExit = false;
    bool bIsGravityOn = true;


    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);
    LARGE_INTEGER PreviousTime;
    QueryPerformanceCounter(&PreviousTime);


    UGameManager GameManager;
    GameManager.Initialize(&Renderer);
    //GameManager.ResetGame();
    UGameUI GameUI;

    RECT rect;
    GetClientRect(hWnd, &rect);
    int centerX = rect.right / 2;
    int centerY = rect.bottom / 2;
    // Main Loop
    while (bIsExit == false)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                bIsExit = true;
                break;
            }
        }





        // Calculating DeltaTime
        ////////////////////////////////////////////
        LARGE_INTEGER CurrentTime;
        QueryPerformanceCounter(&CurrentTime);
        float DeltaTime = static_cast<float>(CurrentTime.QuadPart - PreviousTime.QuadPart) / static_cast<float>(Frequency.QuadPart);
        PreviousTime = CurrentTime;
        if (DeltaTime > 0.1f)
            DeltaTime = 0.1f;

        GameManager.Update(DeltaTime);
        UCharacterPlayer* Player = GameManager.GetPlayer();
        UCharacterEnemy** EnemyList = GameManager.GetEnemyList();
        ////////////////////////////////////////////



        // Player Input & Movement
        ////////////////////////////////////////////
        int EnemyListCount = GameManager.GetEnemyListCount();
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hWnd, &pt);

        static bool bWasEscapePressed = false;
        bool bIsEscapePressed = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;

        if (bIsEscapePressed && !bWasEscapePressed)
        {
            if (GameManager.IsPlaying())
            {
                GameManager.PauseGame();
            }
            else if (GameManager.IsPaused())
            {
                GameManager.ResumeGame();
            }
        }

        bWasEscapePressed = bIsEscapePressed;

        if (GameManager.IsPlaying()) {
            if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState(0x41) & 0x8000) { //왼쪽
                Player->Move({ Player->Location.x - 1, Player->Location.y, 0 }, DeltaTime);
            }
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState(0x44) & 0x8000) { //오른쪽
                Player->Move({ Player->Location.x + 1, Player->Location.y, 0 }, DeltaTime);
            }
            if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(0x57) & 0x8000) { //위
                Player->Move({ Player->Location.x, Player->Location.y + 1, 0 }, DeltaTime);
            }
            if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState(0x53) & 0x8000) { //아래
                Player->Move({ Player->Location.x, Player->Location.y - 1, 0 }, DeltaTime);
            }
            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { //마우스 왼쪽
                if (Player->bIsShoot) {
                    FVector V = { ((float)pt.x / centerX) - 1.f - Player->PlayerOffset.x, 1.f - ((float)pt.y / centerY) - Player->PlayerOffset.y, 0 };
                    V.Normalize();
                    float baseRad = atan2(V.x, V.y);
                    constexpr float DEG2RAD = 3.14159265f / 180.0f;

                    USoundManager::GetInstance().PlaySFX(ESFXType::PlayerShoot);

                    for (INT32 Index = 0; Index < Player->Bullets; ++Index) {
                        float offsetDeg = (Player->Bullets > 1) ? -Player->Radian / 2.0f + Index * (Player->Radian / (Player->Bullets - 1)) : 0.0f;
                        float finalRad = baseRad + (offsetDeg * DEG2RAD);
                        GameManager.SpawnProjectile(Player->Location, { sin(finalRad), cos(finalRad), 0 }, ETypeCharacter::ETC_PlayerProjectile,
                            ETypeProjectile::ETP_Projectile, Player->Damage, Player->Penetration);
                    }

                    Player->bIsShoot = false;
                }
            }
            if (!Player->bIsAxe) {
                for (int i = 0; i < Player->AxeCnt; i++) {
                    float angleRadian = (75.0f + ((float)rand() / RAND_MAX) * 30.0f) * (3.14159265f / 180.0f);
                    FVector Velocity = { cos(angleRadian), sin(angleRadian), 0 };
                    GameManager.SpawnProjectile(Player->Location, Velocity * 2.f, ETypeCharacter::ETC_PlayerProjectile,
                        ETypeProjectile::ETP_Axe, Player->Damage, 999);
                }
                Player->bIsAxe = true;
            }
            Player->UpdateTime(DeltaTime);
            for (INT32 CurrentIndex = 0; CurrentIndex < GameManager.GetEnemyListCount(); ++CurrentIndex)
            {
                if (EnemyList[CurrentIndex] != nullptr && EnemyList[CurrentIndex]->IsActive()) {
                    EnemyList[CurrentIndex]->Move(Player->Location, DeltaTime);
                    if (EnemyList[CurrentIndex]->bIsShoot) {
                        FVector V = { Player->Location.x - EnemyList[CurrentIndex]->Location.x, Player->Location.y - EnemyList[CurrentIndex]->Location.y ,0 };
                        V.Normalize();
                        GameManager.SpawnProjectile(EnemyList[CurrentIndex]->Location, V, ETypeCharacter::ETC_EnemyProjectile,
                            ETypeProjectile::ETP_Projectile, EnemyList[CurrentIndex]->Damage, 1);
                        EnemyList[CurrentIndex]->bIsShoot = false;
                    }
                    else
                        EnemyList[CurrentIndex]->UpdateTime(DeltaTime);
                }
                else {
                    GameManager.AddKill();
                    GameManager.SpawnItem(EnemyList[CurrentIndex]->Location);
                    GameManager.RemoveEnemy(CurrentIndex--);
                }
            }



            UItem** ItemList = GameManager.GetItemList();
            for (INT32 CurrentIndex = 0; CurrentIndex < GameManager.GetItemListCount(); ++CurrentIndex)
            {
                if (ItemList[CurrentIndex] != nullptr && ItemList[CurrentIndex]->IsActive() && 
                    ItemList[CurrentIndex]->bIsMagnet) {
                    UItemEXP* ItemExp = static_cast<UItemEXP*>(ItemList[CurrentIndex]);
                    if(ItemExp->bIsFollow)
                        ItemExp ->Move(Player->Location, DeltaTime);
                }
            }



            UProjectile** ProjectileList = GameManager.GetProjectileList();
            int ProjectileListCount = GameManager.GetProjectileListCount();
            for (INT32 CurrentIndex = 0; CurrentIndex < ProjectileListCount; ++CurrentIndex)
            {
                switch (ProjectileList[CurrentIndex]->ProjectileType) {
                case ETypeProjectile::ETP_Projectile:
                    ProjectileList[CurrentIndex]->Move(ProjectileList[CurrentIndex]->Velocity, DeltaTime);
                    break;
                case ETypeProjectile::ETP_Axe:
                    static_cast<UProjectileAxe*>(ProjectileList[CurrentIndex])->Move(ProjectileList[CurrentIndex]->Velocity, DeltaTime);
                    break;
                }
            }

            int LevelUpCnt = Player->LevelUp();
            if (LevelUpCnt > 0) {
                Player->RandomUpgrade();
                GameManager.SetGameState(EGameState::Upgrade);
            }
        }

        
        
        ////////////////////////////////////////////



        // Physics Process
        HandleCollision(&GameManager,DeltaTime);


        // Rendering Process
        ////////////////////////////////////////////
        Renderer.Prepare();

        DrawBackground(Player, &Renderer, GameManager.IsTitle());

        DrawObjects(&GameManager, &Renderer, GameManager.IsTitle());



        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        GameUI.UIRender(&GameManager);
        DrawUI(Player, &Renderer, GameManager.IsPlaying());

        ImGui::Render();

        /*
        /// ImGui - start
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::SetNextWindowSize(ImVec2(370, 100));


        ImGui::Begin("Jungle Property Window");
        ImGui::Text("Hello Jungle World!");

        ImGui::Checkbox("Gravity", &bIsGravityOn);

        static INT32 TargetNumBalls = UBall::TotalNumBalls;
        if (ImGui::InputInt("Number of balls", &TargetNumBalls))
        {
            if (TargetNumBalls < 0)
            {
                TargetNumBalls = 0;
            }
        }

        while (UBall::TotalNumBalls < TargetNumBalls)
        {
            AddBall(PrimitiveList, CurrentListSize);
        }
        while (UBall::TotalNumBalls > TargetNumBalls)
        {
            RemoveBall(PrimitiveList);
        }

        ImGui::SameLine(0.0f, 20.0f);
        if (ImGui::Button("-"))
        {
            RemoveBall(PrimitiveList);
        }
        ImGui::SameLine(0.0f, 10.0f);
        if (ImGui::Button("+"))
        {
            AddBall(PrimitiveList, CurrentListSize);
        }


        ImGui::End();


        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        /// ImGui - end
        */

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        Renderer.SwapBuffer();
        ////////////////////////////////////////////
    }

    USoundManager::GetInstance().Shutdown();

    // Destroying ImGui
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Releasing URenderer Object
    Renderer.Release();


    return 0;
}
///////////////////////////////////////////////