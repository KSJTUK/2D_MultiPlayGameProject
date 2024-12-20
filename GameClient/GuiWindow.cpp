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
//					Text�� �׸��� ���� Window								//
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
//					     ä���� ���� ������                               //
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

     // �Ʒ����� ���� �޽��� ���
    for (const auto& str : mChatBuffer) {
        ImGui::Text(str.c_str());
    }

    // ��ũ���� �ڵ����� �� �Ʒ��� �̵�
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    ImGui::Separator(); // ���м� �߰�
    ImGui::InputText("##Input", input, IM_ARRAYSIZE(input), ImGuiInputTextFlags_EnterReturnsTrue);
    // Enter �Է� �� �޽����� �߰�
    if (::strlen(input) > 0 and ImGui::IsKeyPressed(ImGuiKey_Enter)) {
        mChatBuffer.Push(std::string{ input });
        ::memset(input, 0, 512); // �Է� �ʵ� �ʱ�ȭ
    }
    // ��ũ���� �� �Ʒ��� ����
    ImGui::SetScrollHereY(1.0f);
}
