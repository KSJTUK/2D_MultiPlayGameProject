#pragma once

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

    void SetTitle(std::string_view title);
    void SetWindowSize(const ImVec2& size);
    void SetBgColor(const ImVec4& color);
    void SetBgAlpha(const float alpha);

public:
    virtual void UpdateContents();

    void Render();

private:
    std::string mTitle{ "" };
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

class TextWindow : public GuiWindow {
public:
    TextWindow();
    ~TextWindow();

public:

private:

};