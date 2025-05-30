#include "pch.h"
#include "Window.h"
#include "Resource.h"
#include "GameFrame.h"
#include "Camera.h"
#include "Utils.h"
#include "Timer.h"

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							WndProc                                     //
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

extern std::unique_ptr<GameFrame> gGameFramework;

LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) {
        return true;
    }

    RAWINPUT raw;
    static bool mouseDown = false;
    static float mouseSensitive = 3.3f;
    switch (message) {
    case WM_CREATE:
    {
        RAWINPUTDEVICE Rid;
        Rid.usUsagePage = 0x01; // Generic desktop controls
        Rid.usUsage = 0x02;     // Mouse
        Rid.dwFlags = 0;        // 0 = receive input even when not in focus (use RIDEV_INPUTSINK if needed)
        Rid.hwndTarget = hWnd;  // 메시지를 받을 윈도우 핸들

        if (!RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE))) {
            OutputDebugString(L"Failed to register raw input device.\n");
        }
        break;
    }

    case WM_KEYUP:
        switch (wParam) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_KEYDOWN:
        break;

    case WM_LBUTTONDOWN:
    {
        mouseDown = true;
        break;
    }

    case WM_LBUTTONUP:
    {
        mouseDown = false;
        break;
    }

    case WM_RBUTTONUP:
    {
        auto x = GET_X_LPARAM(lParam);
        auto y = GET_Y_LPARAM(lParam);

        decltype(auto) camera = gGameFramework->GetMainCamera();
        auto size = camera->GetMainWindowSize();
        auto screenPos = Position{ x, y };
        camera->SetFollowTarget(camera->ScreenPosToWorld(screenPos), 3.0f);
        break;
    }

    case WM_INPUT:
    {
        if (not mouseDown) {
            break;
        }

        UINT dwSize{ };
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
        BYTE* lpb = new BYTE[dwSize];
        if (lpb == nullptr) return 0;

        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
            OutputDebugString(L"GetRawInputData doesn't return correct size !\n");
        }

        raw = *(RAWINPUT*)lpb;

        if (raw.header.dwType == RIM_TYPEMOUSE) {
            float dx = -raw.data.mouse.lLastX / mouseSensitive;
            float dy = -raw.data.mouse.lLastY / mouseSensitive;
            
            decltype(auto) camera = gGameFramework->GetMainCamera();
            camera->SetPosition(camera->GetPosition() + Position{ dx, dy });
        }

        delete[] lpb;
        break;
    }

    case WM_MOUSEWHEEL:
    {
        float delta = (-(float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA + (float)WHEEL_DELTA) / (float)WHEEL_DELTA;
        gGameFramework->GetMainCamera()->Zoom(delta, TEMP_MAP_AREA.x);
        break;
    }

    case WM_SIZE:
        if (gGameFramework) {
            gGameFramework->ResetSize();
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							Widow                                       //
//					   메인 윈도우 생성       							//
//																		//
//////////////////////////////////////////////////////////////////////////

Window::Window(HINSTANCE instance, std::wstring_view name, WindowMode mode, const Size& size) : mSize{ size }  {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_GAMECLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = name.data();
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    mHandle = CreateWindowW(name.data(), name.data(), WS_OVERLAPPEDWINDOW,
        100, 100, mSize.width, mSize.height, nullptr, nullptr, instance, nullptr);

    if (nullptr == mHandle) {
        MessageBoxA(nullptr, "Window.cpp\nclass Window's Constructor Failure", "Initialization Window Failure", MB_OK | MB_ICONERROR);
        PostQuitMessage(EXIT_FAILURE);
    }

    __analysis_assume(mHandle != nullptr); /* nullptr 검사를 위에서 하고있음에도 에러가 나서 넣은 코드 */

    ShowWindow(mHandle, TRUE);
    UpdateWindow(mHandle);

    SetWindowLong(mHandle, GWL_STYLE,
        GetWindowLong(mHandle, GWL_STYLE) & ~WS_MINIMIZEBOX);
    SetWindowLong(mHandle, GWL_STYLE,
        GetWindowLong(mHandle, GWL_STYLE) & ~WS_MAXIMIZEBOX);
    EnableMenuItem(GetSystemMenu(mHandle, FALSE), SC_CLOSE,
        MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}

Window::~Window() { 
    DestroyWindow(mHandle);
}

HWND Window::GetHandle() const {
    return mHandle;
}

Size Window::GetSize() const {
    return mSize;
}

RECT Window::GetRect() const {
    RECT rc;
    GetClientRect(mHandle, &rc);
    return rc;
}

RECT Window::GetScreenRect() const {
    RECT rc = GetRect();

    long halfW = (rc.right - rc.left) / 2;
    long halfH = (rc.bottom - rc.top) / 2;

    rc.left -= halfW;
    rc.right -= halfW;
    rc.top -= halfH;
    rc.bottom -= halfH;
    return rc;
}

RECT Window::GetScreenRect(int offsetW, int offsetH) const
{
    RECT rc = GetRect();

    long halfW = (rc.right - rc.left) / 2;
    long halfH = (rc.bottom - rc.top) / 2;

    rc.left -= halfW + offsetW;
    rc.right -= halfW + offsetW;
    rc.top -= halfH + offsetH;
    rc.bottom -= halfH + offsetH;
    return rc;
}
