#include "pch.h"
#include "TextWriter.h"
#include "Utils.h"

TextWriter::TextWriter() : mFontSize{ DefaultFontSize } {
    HRESULT hr;
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory3), reinterpret_cast<IUnknown**>(mWriteFactory.GetAddressOf()));
    CheckHR(hr, std::source_location::current());

    hr = mWriteFactory->CreateTextFormat(
        L"Verdana",                     // 폰트 패밀리 이름의 문자열
        NULL,                           // 폰트 컬렉션 객체, NULL=시스템 폰트 컬렉션
        DWRITE_FONT_WEIGHT_NORMAL,      // 폰트 굵기. LIGHT, NORMAL, BOLD 등.
        DWRITE_FONT_STYLE_NORMAL,       // 폰트 스타일. NORMAL, OBLIQUE, ITALIC.
        DWRITE_FONT_STRETCH_NORMAL,     // 폰트 간격. CONDENSED, NORMAL, MEDIUM, EXTEXDED 등.
        mFontSize,                      // 폰트 크기.
        L"ko-kr",                       // 로케일을 문자열로 명시.  영어-미국=L"en-us", 한국어-한국=L"ko-kr"
        reinterpret_cast<IDWriteTextFormat**>(mDefaultFormat.GetAddressOf())
    );

    CheckHR(hr, std::source_location::current());
    mDefaultFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

void TextWriter::WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str) {
    size_t textBoxWidth = str.length() * mFontSize;
    RectF textBox = D2D1::RectF(pos.x - textBoxWidth * 0.5f, pos.y - mFontSize * 0.5f, pos.x + textBoxWidth * 0.5f, pos.y + mFontSize  * 0.5f);

    renderTarget->DrawTextW(str.data(), str.length(), mDefaultFormat.Get(), textBox, SolidBrush::blackBrush.Get());
}

void TextWriter::WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str, D2D1::ColorF::Enum bkColor) {
    size_t textBoxWidth = str.length() * mFontSize;
    RectF textBox = D2D1::RectF(pos.x - textBoxWidth * 0.5f, pos.y - mFontSize, pos.x + textBoxWidth * 0.5f, pos.y + mFontSize * 0.5f);

    ID2D1SolidColorBrush* brush;
    renderTarget->CreateSolidColorBrush(Color(bkColor), &brush);

    renderTarget->FillRectangle(textBox, brush);

    renderTarget->DrawTextW(str.data(), str.length(), mDefaultFormat.Get(), textBox, SolidBrush::blackBrush.Get());

    brush->Release();
}
