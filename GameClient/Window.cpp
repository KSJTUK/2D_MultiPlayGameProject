#include "pch.h"
#include "Window.h"
#include "Resource.h"


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							WndProc                                     //
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_KEYUP:
        switch (wParam) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
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
