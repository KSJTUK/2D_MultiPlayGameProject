#include "pch.h"
#include "GuiWindow.h"
#include "Camera.h"

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							GuiWindow                                   //
//					ImGui 사용한 GUI용 클래스								//
//																		//
//////////////////////////////////////////////////////////////////////////

GuiWindow::GuiWindow() { }

GuiWindow::~GuiWindow() { }

void GuiWindow::EnableWindowFlag(ImGuiWindowFlags flag) {
    mWindowFlags |= flag;
}

void GuiWindow::DisableWindowFlag(ImGuiWindowFlags flag) {
    mWindowFlags &= ~flag;
}

std::string GuiWindow::GetTitle() const {
    return mTitle;
}

ImVec2 GuiWindow::GetWindowSize() const {
    return mWindowSize;
}

ImVec4 GuiWindow::GetBgColor() const {
    return mBgColor;
}

float GuiWindow::GetBgAlpha() const {
    return mBgAlpha;
}

void GuiWindow::SetMainWindowSize(Size windowSize) {
    mMainWindowSize = windowSize;
}

void GuiWindow::SetTitle(std::string_view title) {
    mTitle = title;
}

void GuiWindow::SetWindowSize(const ImVec2& size) {
    mWindowSize = size;
}

void GuiWindow::SetBgColor(const ImVec4& color) {
    mBgColor = color;
}

void GuiWindow::SetBgAlpha(const float alpha) {
    mBgAlpha = alpha;
}

void GuiWindow::SetPosition(const ImVec2& pos) {
    ImGui::SetWindowPos(pos);
}

void GuiWindow::SetPosCameraLeftBottom(Size windowSize) {
    mWindowSize.x = windowSize.width * 0.25f;
    mWindowSize.y = windowSize.height * 0.33f;
    auto ySize = mWindowSize.y; 
    auto yPos = windowSize.height - ySize;

    SetPosition(ImVec2{ 0.0f, yPos });
}

void GuiWindow::UpdateContents() {  }

void GuiWindow::Render() {
    ImGui::SetNextWindowBgAlpha(mBgAlpha);
    ImGui::SetNextWindowSize(mWindowSize);

    bool pOpen = false;
    ImGui::Begin("Chat", &pOpen, mWindowFlags);
    SetPosCameraLeftBottom(mMainWindowSize);

    UpdateContents();

    ImGui::End();
}

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							TextWindow                                  //
//					Text를 그리기 위한 Window								//
//																		//
//////////////////////////////////////////////////////////////////////////

TextWindow::TextWindow() : mTextBuffer{ } { }

TextWindow::~TextWindow() { }

void TextWindow::InputText(std::string_view str, bool nextSameLine) {
    mTextBuffer.Push(TextInfo{ std::string{ str }, nextSameLine});
}

void TextWindow::UpdateContents() {
    for (const auto& [str, sameLine] : mTextBuffer) {
        ImGui::Text(str.c_str());
        if (sameLine) {
            ImGui::SameLine();
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							ChatWindow                                  //
//					     채팅을 위한 윈도우                               //
//																		//
//////////////////////////////////////////////////////////////////////////

ChatWindow::ChatWindow() { }

ChatWindow::~ChatWindow() { }

void ChatWindow::InputText(std::wstring_view str) { 
    mChatBuffer.Push(std::wstring{ str });
}

void ChatWindow::UpdateContents() {
    static wchar_t input[BUFSIZ]{ };
    static char mbInput[BUFSIZ * sizeof(wchar_t)]{};
    ImGui::BeginChild("ChatScrollRegion", ImVec2{ 0, -25 });

     // 아래에서 위로 메시지 출력
    char mbBuffer[BUFSIZ * sizeof(wchar_t)]{ };
    for (const auto& str : mChatBuffer) {
        auto result = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), static_cast<int32_t>(str.size()), mbBuffer, BUFSIZ * sizeof(wchar_t), nullptr, nullptr);
        if (0 == result) {
            auto errorCode = GetLastError();
            MessageBox(nullptr, nullptr, nullptr, MB_OK);
            return;
        }

        mbBuffer[result] = 0;
        ImGui::Text(mbBuffer);
    }

    // 스크롤을 자동으로 맨 아래로 이동
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::Separator(); // 구분선 추가
    ImGui::InputText("##Input", mbInput, IM_ARRAYSIZE(mbInput), ImGuiInputTextFlags_EnterReturnsTrue);
    // Enter 입력 시 메시지를 추가
    if (::strlen(mbInput) > 0 and ImGui::IsKeyPressed(ImGuiKey_Enter)) {
        auto result = MultiByteToWideChar(CP_UTF8, 0, mbInput, static_cast<int32_t>(::strlen(mbInput)), input, BUFSIZ);
        if (0 == result) {
            auto errorCode = GetLastError();
            MessageBox(nullptr, std::wstring{ std::wstring{ L"Error Code Is : " } + std::to_wstring(errorCode) }.c_str(), nullptr, MB_OK);
            return;
        }

        mChatBuffer.Push(std::wstring{ input });
        ::memset(input, 0, BUFSIZ * 2); // 입력 필드 초기화
        ::memset(mbInput, 0, BUFSIZ * 2); // 입력 필드 초기화
    }
}
