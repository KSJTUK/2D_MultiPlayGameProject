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

void GuiWindow::UpdateContents() { }

void GuiWindow::Render() {
    ImGui::SetNextWindowBgAlpha(mBgAlpha);
    ImGui::SetNextWindowSize(mWindowSize);

    bool pOpen = false;
    ImGui::Begin("Hello Imgui", &pOpen, mWindowFlags);

    UpdateContents();

    ImGui::End();
}
