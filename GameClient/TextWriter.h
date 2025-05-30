#pragma once

inline constexpr Position STATIC_DEBUG_TEXT_FRAME = Position{ 300.0f, -300.0f };
static constexpr float DEFAULT_FONT_SIZE = 15.0f;
inline constexpr uint32_t DEFAULT_FONT_KEY = 0;

struct FontProperties {
    std::wstring fontFamilyName{ };
    std::wstring locale{ L"ko-kr" };
    DWRITE_FONT_WEIGHT weight{ DWRITE_FONT_WEIGHT_NORMAL };
    DWRITE_FONT_STYLE style{ DWRITE_FONT_STYLE_NORMAL };
    DWRITE_FONT_STRETCH stretch{ DWRITE_FONT_STRETCH_NORMAL };
    float size{ DEFAULT_FONT_SIZE };
    DWRITE_TEXT_ALIGNMENT alignment{ DWRITE_TEXT_ALIGNMENT_CENTER };
    DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment{ DWRITE_PARAGRAPH_ALIGNMENT_CENTER };
};

class TextWriter {
private:
    struct TextFormat {
        float fontSize{ DEFAULT_FONT_SIZE };
        ComPtr<IDWriteTextFormat3> font{ nullptr };
    };

public:
    TextWriter();
    ~TextWriter();

public:
    bool SetFont(uint32_t fontKey);
    bool LoadFontFromFile(uint32_t fontKey, const std::filesystem::path& path, FontProperties& fontProp);

    /// 항상 ID2D1RenderTarget의 BeginDraw 호출후, EndDraw 호출 전 사용할 것
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str);
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& stPos, std::initializer_list<const char*> strs);
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& stPos, std::initializer_list<std::wstring_view> strs);

    template <typename It> requires std::is_convertible_v<typename std::iterator_traits<It>::value_type, std::string_view>
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& stPos, It strBeg, It strEnd);
    template <typename It> requires std::is_convertible_v<typename std::iterator_traits<It>::value_type, std::wstring_view>
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& stPos, It strBeg, It strEnd);

    /// 항상 ID2D1RenderTarget의 BeginDraw 호출후, EndDraw 호출 전 사용할 것
    void WriteText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str, D2D1::ColorF::Enum bkColor);

    /// 항상 ID2D1RenderTarget의 BeginDraw 호출후, EndDraw 호출 전 사용할 것
    void WriteColorText(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const Position& pos, std::wstring_view str, D2D1::ColorF::Enum textColor);

    ComPtr<IDWriteFactory5> GetWriteFactory();

private:
    ComPtr<IDWriteFactory5> mWriteFactory;

    std::unordered_map<uint32_t, std::shared_ptr<TextFormat>> mTextFormatMap;
    std::shared_ptr<TextFormat> mCurrentFormat;
};