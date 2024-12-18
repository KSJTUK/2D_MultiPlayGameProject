#pragma once

constexpr Size DEFAULT_WIDNOW_SIZE{ 1080, 800 };

enum WindowMode {
    windowed,
    full,
};

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							Window                                      //
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

class Window {
public:
    Window(HINSTANCE instance, std::wstring_view name, WindowMode mode=windowed, const Size& size = DEFAULT_WIDNOW_SIZE);
    ~Window();

public:
    HWND GetHandle() const;
    Size GetSize() const;
    RECT GetRect() const;
    RECT GetScreenRect() const;
    RECT GetScreenRect(int offsetW, int offsetH) const;

private:
    HWND mHandle;
    Size mSize;
};