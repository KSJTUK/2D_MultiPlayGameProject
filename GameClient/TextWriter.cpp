#include "pch.h"
#include "TextWriter.h"
#include "Utils.h"

TextWriter::TextWriter() {   
    HRESULT hr{ };
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5), reinterpret_cast<IUnknown**>(mWriteFactory.GetAddressOf()));
    CheckHR(hr);

    mTextFormatMap.emplace(DEFAULT_FONT_KEY, std::make_shared<TextFormat>(DEFAULT_FONT_SIZE, ComPtr<IDWriteTextFormat3>()));
    auto& formatPtr = mTextFormatMap.at(DEFAULT_FONT_KEY);

    hr = mWriteFactory->CreateTextFormat(
        L"Verdana",                             // 폰트 패밀리 이름의 문자열
        NULL,                                   // 폰트 컬렉션 객체, NULL=시스템 폰트 컬렉션
        DWRITE_FONT_WEIGHT_NORMAL,              // 폰트 굵기. LIGHT, NORMAL, BOLD 등.
        DWRITE_FONT_STYLE_NORMAL,               // 폰트 스타일. NORMAL, OBLIQUE, ITALIC.
        DWRITE_FONT_STRETCH_NORMAL,             // 폰트 간격. CONDENSED, NORMAL, MEDIUM, EXTEXDED 등.
        formatPtr->fontSize,                    // 폰트 크기.
        L"ko-kr",                               // 로케일을 문자열로 명시.  영어-미국=L"en-us", 한국어-한국=L"ko-kr"
        reinterpret_cast<IDWriteTextFormat**>(formatPtr->font.GetAddressOf())
    );

    CheckHR(hr);
    formatPtr->font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

    SetFont(DEFAULT_FONT_KEY);
}

TextWriter::~TextWriter() { }

bool TextWriter::SetFont(uint32_t fontKey) {
    auto it = mTextFormatMap.find(fontKey);
    if (mTextFormatMap.end() == it or nullptr == it->second->font) {
        return false;
    }

    mCurrentFormat = mTextFormatMap.at(fontKey);
    return true;
}

bool TextWriter::LoadFontFromFile(uint32_t fontKey, const std::filesystem::path& path, FontProperties& fontProp) {
    ComPtr<IDWriteFontSetBuilder1> fontSetBuilder{};
    CheckHR(mWriteFactory->CreateFontSetBuilder(&fontSetBuilder));

    ComPtr<IDWriteFontFile> fontFile{};
    CheckHR(mWriteFactory->CreateFontFileReference(path.c_str(), nullptr, &fontFile));

    CheckHR(fontSetBuilder->AddFontFile(fontFile.Get()));

    ComPtr<IDWriteFontSet4> fontSet{};
    CheckHR(fontSetBuilder->CreateFontSet(reinterpret_cast<IDWriteFontSet**>(fontSet.GetAddressOf())));

    ComPtr<IDWriteFontCollection1> fontCollection{};
    CheckHR(mWriteFactory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection));

    ComPtr<IDWriteTextFormat3> format{};
    CheckHR(mWriteFactory->CreateTextFormat(
        fontProp.fontFamilyName.c_str(),
        fontCollection.Get(),
        fontProp.weight,
        fontProp.style,
        fontProp.stretch,
        fontProp.size,
        fontProp.locale.c_str(),
        reinterpret_cast<IDWriteTextFormat**>(format.GetAddressOf())
    ));

    format->SetTextAlignment(fontProp.alignment);
    format->SetParagraphAlignment(fontProp.paragraphAlignment);

    mTextFormatMap.emplace(fontKey, std::make_shared<TextFormat>(fontProp.size, format));

    return true;
}

void TextWriter::WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str) {
    SizeF textBoxSize{ static_cast<float>(str.length() * mCurrentFormat->fontSize), static_cast<float>(mCurrentFormat->fontSize) };
    RectF textBox = CreateRectF(pos, textBoxSize);

    renderTarget->DrawTextW(str.data(), static_cast<long>(str.length()), mCurrentFormat->font.Get(), textBox, SolidBrush::GetBrush(D2D1::ColorF::Black));
}

void TextWriter::WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str, D2D1::ColorF::Enum bkColor) {
    SizeF textBoxSize{ static_cast<float>(str.length() * mCurrentFormat->fontSize), static_cast<float>(mCurrentFormat->fontSize) };
    RectF textBox = CreateRectF(pos, textBoxSize);

    renderTarget->FillRectangle(textBox, SolidBrush::GetBrush(bkColor));

    auto complementaryColor = ComplementaryColorRGB(bkColor);
    renderTarget->DrawTextW(str.data(), static_cast<long>(str.length()), mCurrentFormat->font.Get(), textBox, SolidBrush::GetBrush(complementaryColor));
}

void TextWriter::WriteColorText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str, D2D1::ColorF::Enum textColor) {
    SizeF textBoxSize{ static_cast<float>(str.length() * mCurrentFormat->fontSize), static_cast<float>(mCurrentFormat->fontSize) };
    RectF textBox = CreateRectF(pos, textBoxSize);

    renderTarget->DrawTextW(str.data(), static_cast<long>(str.length()), mCurrentFormat->font.Get(), textBox, SolidBrush::GetBrush(textColor));
}

ComPtr<IDWriteFactory5> TextWriter::GetWriteFactory() {
    return mWriteFactory;
}