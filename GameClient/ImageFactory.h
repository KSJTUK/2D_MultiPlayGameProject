#pragma once

class ImageFactory {
public:
    ImageFactory() = delete;
    ~ImageFactory() = delete;

public:
    static bool Init(ComPtr<ID2D1Factory1> factory, ComPtr<ID2D1HwndRenderTarget> renderTarget);
    static void Destroy();
    static ComPtr<ID2D1Bitmap> ImageLoad(const std::wstring_view path);

private:
    inline static IWICImagingFactory* mWICFactory{ nullptr };             /* WIC 객체들을 생성할 Factory */
    inline static ComPtr<ID2D1HwndRenderTarget> mRenderTarget{ nullptr };
};