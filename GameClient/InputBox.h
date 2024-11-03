#pragma once

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							    InputBox                                //
//					         입력 박스 생성       					    //
//																		//
//////////////////////////////////////////////////////////////////////////

#define WS_INPUT (WS_CHILD | WS_BORDER | WS_VISIBLE)

class InputBox {
public:
    InputBox(const ComPtr<ID2D1Factory>& factory, HINSTANCE instance, HWND parent);
    ~InputBox();

public:


private:
    std::wstring mClassName;
    std::wstring mTitle;
    HWND mWindowHandle;

    ComPtr<ID2D1HwndRenderTarget> mRenderTarget;
};
