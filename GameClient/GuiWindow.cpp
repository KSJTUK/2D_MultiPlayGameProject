#include "pch.h"
#include "GuiWindow.h"

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

void GuiWindow::UpdateContents() {  }

void GuiWindow::Render() {
    ImGui::SetNextWindowBgAlpha(mBgAlpha);
    ImGui::SetNextWindowSize(mWindowSize);

    bool pOpen = false;
    ImGui::Begin("Hello Imgui", &pOpen, mWindowFlags);

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

void ChatWindow::InputText(std::string_view str) { 
    mChatBuffer.Push(std::string{ str });
}

void ChatWindow::UpdateContents() {
    static char input[512]{ };
    ImGui::BeginChild("ChatScrollRegion", ImVec2{ 0, -30 });

     // 아래에서 위로 메시지 출력
    for (const auto& str : mChatBuffer) {
        ImGui::Text(str.c_str());
    }

    // 스크롤을 자동으로 맨 아래로 이동
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::Separator(); // 구분선 추가
    ImGui::InputText("##Input", input, IM_ARRAYSIZE(input), ImGuiInputTextFlags_EnterReturnsTrue);
    // Enter 입력 시 메시지를 추가
    if (::strlen(input) > 0 and ImGui::IsKeyPressed(ImGuiKey_Enter)) {
        mChatBuffer.Push(std::string{ input });
        ::memset(input, 0, 512); // 입력 필드 초기화
    }
    // 스크롤을 맨 아래로 유지
    ImGui::SetScrollHereY(1.0f);
}
