#pragma once

class TextWriter {
    static constexpr int DefaultFontSize = 15;

public:
    TextWriter();

public:
    /// 항상 ID2D1RenderTarget의 BeginDraw 호출후, EndDraw 호출 전 사용할 것
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str);

    /// 항상 ID2D1RenderTarget의 BeginDraw 호출후, EndDraw 호출 전 사용할 것
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str, D2D1::ColorF::Enum bkColor);

    ComPtr<IDWriteFactory3> GetWriteFactory();

private:
    ComPtr<IDWriteFactory3> mWriteFactory;
    ComPtr<IDWriteTextFormat3> mDefaultFormat;

    float mFontSize;
};