#pragma once

class TextWriter {
    static constexpr int DefaultFontSize = 15;

public:
    TextWriter();

public:
    /// �׻� ID2D1RenderTarget�� BeginDraw ȣ����, EndDraw ȣ�� �� ����� ��
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str);

    /// �׻� ID2D1RenderTarget�� BeginDraw ȣ����, EndDraw ȣ�� �� ����� ��
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str, D2D1::ColorF::Enum bkColor);

    ComPtr<IDWriteFactory3> GetWriteFactory();

private:
    ComPtr<IDWriteFactory3> mWriteFactory;
    ComPtr<IDWriteTextFormat3> mDefaultFormat;

    float mFontSize;
};