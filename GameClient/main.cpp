#include "pch.h"
#include "Resource.h"
#include "GameFrame.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMECLIENT));

    MSG msg;
    GameFrame gameFramework{ hInstance };

    // 기본 메시지 루프입니다:
    while (true) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (WM_QUIT == msg.message) {
                break;
            }

            if (WM_SIZE == msg.message) {
                gameFramework.ResetSize();
            }

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else {
            /* TODO : 게임 루프(로직)가 들어갈 부분 */
            gameFramework.Update();
            gameFramework.Render();
        }
    }

    return (int)msg.wParam;
}