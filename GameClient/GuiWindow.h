﻿#pragma once

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							GuiWindow                                   //
//					ImGui 사용한 GUI용 클래스								//
//																		//
//////////////////////////////////////////////////////////////////////////

class GuiWindow {
    inline static constexpr ImVec2 DEFAULT_SIZE{ 100, 20 };

public:
    GuiWindow();
    ~GuiWindow();

public:
    void EnableWindowFlag(ImGuiWindowFlags flag);
    void DisableWindowFlag(ImGuiWindowFlags flag);

    std::string GetTitle() const;
    ImVec2 GetWindowSize() const;
    ImVec4 GetBgColor() const;
    float GetBgAlpha() const;
    
    void SetMainWindowSize(Size windowSize);
    void SetTitle(std::string_view title);
    void SetWindowSize(const ImVec2& size);
    void SetBgColor(const ImVec4& color);
    void SetBgAlpha(const float alpha);

    void SetPosition(const ImVec2& pos);
    void SetPosCameraLeftBottom(Size windowSize);

public:
    virtual void UpdateContents();

    void Render();

private:
    std::string mTitle{ "" };
    Size mMainWindowSize{ DEFAULT_WINDOW_SIZE };

    ImGuiWindowFlags mWindowFlags{ };
    ImVec2 mWindowSize{ DEFAULT_SIZE };
    ImVec4 mBgColor{ };
    float mBgAlpha{ 1.0f };
};

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							TextWindow                                  //
//					Text를 그리기 위한 Window								//
//																		//
//////////////////////////////////////////////////////////////////////////

#include "InputBuffer.h"

struct TextInfo {
    std::string str{ };
    bool nextSameLine{ false };
};

class TextWindow : public GuiWindow {
    inline static constexpr size_t MAX_TEXT = 100;

public:
    TextWindow();
    ~TextWindow();

public:
    void InputText(std::string_view str, bool nextSameLine =false);
    virtual void UpdateContents() override final;

private:
    InputBuffer<TextInfo, MAX_TEXT> mTextBuffer{ };
};

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							ChatWindow                                  //
//					     채팅을 위한 윈도우                               //
//																		//
//////////////////////////////////////////////////////////////////////////

class ChatWindow : public GuiWindow {
    inline static constexpr size_t MAX_CHAT = 100;
public:
    ChatWindow();
    ~ChatWindow();

public:
    void InputText(std::wstring_view str);
    virtual void UpdateContents() override final;

private:
    InputBuffer<std::wstring, MAX_CHAT> mChatBuffer{ };
};