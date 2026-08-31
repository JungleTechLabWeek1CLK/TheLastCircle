///////////////////////////////////////////////


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

///////////////////////////////////////////////



// Initialize Ball Number to 0
int UBall::TotalNumBalls = 0;




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

    UPrimitive** PrimitiveList = nullptr;
    INT32 CurrentListSize = 8;
    PrimitiveList = new UPrimitive * [CurrentListSize];

    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);
    LARGE_INTEGER PreviousTime;
    QueryPerformanceCounter(&PreviousTime);

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

        // Physics Process
        ////////////////////////////////////////////
        LARGE_INTEGER CurrentTime;
        QueryPerformanceCounter(&CurrentTime);
        float DeltaTime = static_cast<float>(CurrentTime.QuadPart - PreviousTime.QuadPart) / static_cast<float>(Frequency.QuadPart);
        PreviousTime = CurrentTime;
        if (DeltaTime > 0.1f)
            DeltaTime = 0.1f;


        if (bIsGravityOn)
        {
            ApplyGravity(PrimitiveList, DeltaTime);
        }

        HandleCollision(PrimitiveList, DeltaTime);

        MoveBalls(PrimitiveList, DeltaTime);
        ////////////////////////////////////////////



        // Rendering Process
        ////////////////////////////////////////////
        Renderer.Prepare();
        DrawBalls(PrimitiveList, &Renderer);



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

        Renderer.SwapBuffer();
        ////////////////////////////////////////////
    }
    // Releasing UBalls
    for (INT32 CurrentIndex = 0, OriginalBallNumnber = UBall::TotalNumBalls; CurrentIndex < OriginalBallNumnber; ++CurrentIndex)
    {
        delete PrimitiveList[CurrentIndex];
    }
    delete[] PrimitiveList;


    // Destroying ImGui
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Releasing URenderer Object
    Renderer.Release();


    return 0;
}
///////////////////////////////////////////////