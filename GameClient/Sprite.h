#pragma once

class Sprite {
public:
    Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::string_view path, Size maxFrame);
    Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::wstring_view path, Size maxFrame);
    Sprite(ComPtr<ID2D1Bitmap> image, Size maxFrame);

public:
    void AdvanceFrame();
    void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarger, D2D1_POINT_2F position);

public:
    ComPtr<ID2D1Bitmap> mImage;
    Size mFrameSize;
    unsigned short mFrameCount;
    Size mMaxFrameCount;
};