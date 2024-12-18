#include "pch.h"
#include "TextWriter.h"
#include "Utils.h"

TextWriter::TextWriter() : mFontSize{ DefaultFontSize } {
    HRESULT hr{ };
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory3), reinterpret_cast<IUnknown**>(mWriteFactory.GetAddressOf()));
    CheckHR(hr, std::source_location::current());

    hr = mWriteFactory->CreateTextFormat(
        L"Verdana",                             // 폰트 패밀리 이름의 문자열
        NULL,                                   // 폰트 컬렉션 객체, NULL=시스템 폰트 컬렉션
        DWRITE_FONT_WEIGHT_NORMAL,              // 폰트 굵기. LIGHT, NORMAL, BOLD 등.
        DWRITE_FONT_STYLE_NORMAL,               // 폰트 스타일. NORMAL, OBLIQUE, ITALIC.
        DWRITE_FONT_STRETCH_NORMAL,             // 폰트 간격. CONDENSED, NORMAL, MEDIUM, EXTEXDED 등.
        mFontSize,                              // 폰트 크기.
        L"ko-kr",                               // 로케일을 문자열로 명시.  영어-미국=L"en-us", 한국어-한국=L"ko-kr"
        reinterpret_cast<IDWriteTextFormat**>(mDefaultFormat.GetAddressOf())
    );

    CheckHR(hr, std::source_location::current());
    mDefaultFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

void TextWriter::WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str) {
    SizeF textBoxSize{ static_cast<float>(str.length() * mFontSize), static_cast<float>(mFontSize) };
    RectF textBox = CreateRectF(pos, textBoxSize);

    renderTarget->DrawTextW(str.data(), static_cast<long>(str.length()), mDefaultFormat.Get(), textBox, SolidBrush::GetBrush(D2D1::ColorF::Black));
}

void TextWriter::WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str, D2D1::ColorF::Enum bkColor) {
    SizeF textBoxSize{ static_cast<float>(str.length() * mFontSize), static_cast<float>(mFontSize) };
    RectF textBox = CreateRectF(pos, textBoxSize);

    renderTarget->FillRectangle(textBox, SolidBrush::GetBrush(bkColor));

    auto complementaryColor = ComplementaryColorRGB(bkColor);
    renderTarget->DrawTextW(str.data(), static_cast<long>(str.length()), mDefaultFormat.Get(), textBox, SolidBrush::GetBrush(complementaryColor));
}
