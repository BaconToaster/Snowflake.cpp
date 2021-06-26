#include <iostream>
#include <windows.h>
#include <thread>

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include "Snowflake.cpp/Snowflake.hpp"

#define TITLE "Snowflake.cpp"
#define TITLEW TEXT(TITLE)

// you need those for snowflake
#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080

#define SNOW_LIMIT 300

std::vector<Snowflake::Snowflake> snow;

bool loader_active = true;
HWND main_hwnd = nullptr;

LPDIRECT3DDEVICE9        g_pd3dDevice;
D3DPRESENT_PARAMETERS    g_d3dpp;
LPDIRECT3D9              g_pD3D;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.hDeviceWindow = hWnd;
    g_d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    g_d3dpp.BackBufferWidth = WINDOW_WIDTH;
    g_d3dpp.BackBufferHeight = WINDOW_HEIGHT;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;
    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

void Style()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_WindowBg] = ImColor(34, 34, 34, 50);
    style.Colors[ImGuiCol_TitleBg] = ImColor(59, 59, 59, 255);
    style.Colors[ImGuiCol_TitleBgActive] = ImColor(59, 59, 59, 255);
    style.Colors[ImGuiCol_FrameBg] = ImColor(59, 59, 59, 137);
    style.Colors[ImGuiCol_Button] = ImColor(98, 98, 98, 102);
    style.Colors[ImGuiCol_ButtonHovered] = ImColor(98, 98, 98, 255);
    style.Colors[ImGuiCol_ButtonActive] = ImColor(81, 81, 81, 255);
}

int main()
{
    FreeConsole();

    // Create application window
    WNDCLASSEXW wc;
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = TITLEW;
    wc.lpszClassName = L"Snowflake.cpp";
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

    RegisterClassExW(&wc);
    main_hwnd = CreateWindowExW(NULL, wc.lpszClassName, TITLEW, WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (WINDOW_WIDTH / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (WINDOW_HEIGHT / 2), WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0);

    // Initialize Direct3D
    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ShowWindow(main_hwnd, SW_SHOW);
    UpdateWindow(main_hwnd);

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr; //crutial for not leaving the imgui.ini file

    io.Fonts->AddFontDefault();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);
    Style();

    POINT mouse;
    RECT rc = { 0 };
    GetWindowRect(main_hwnd, &rc);

    // create the snow flakes
    Snowflake::CreateSnowFlakes(snow, SNOW_LIMIT, 5.f/*minimum size*/, 25.f/*maximum size*/, 0/*imgui window x position*/, 0/*imgui window y position*/, WINDOW_WIDTH, WINDOW_HEIGHT, Snowflake::vec3(0.f, 0.005f)/*gravity*/, IM_COL32(255, 255, 255, 100)/*color*/);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
            ImGui::SetNextWindowBgAlpha(1.0f);

            ImGui::Begin(TITLE, &loader_active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
            {
                GetWindowRect(main_hwnd, &rc);

                if (ImGui::GetWindowPos().x != 0 || ImGui::GetWindowPos().y != 0)
                {
                    MoveWindow(main_hwnd, rc.left + ImGui::GetWindowPos().x, rc.top + ImGui::GetWindowPos().y, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
                    ImGui::SetWindowPos(ImVec2(0.f, 0.f));
                }

                GetCursorPos(&mouse);
                // render this before anything else so it is the background
                Snowflake::Update(snow, Snowflake::vec3(mouse.x, mouse.y)/*mouse x and y*/, Snowflake::vec3(rc.left, rc.top)/*hWnd x and y positions*/); // you can change a few things inside the update function

                // render other stuff

            }
            ImGui::End();
        }
        ImGui::EndFrame();


        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
        if (!loader_active) {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}