#pragma once

class Sprite {
public:
    Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::string_view path, Size maxFrame, size_t frameEndWidth=0);
    Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::wstring_view path, Size maxFrame, size_t frameEndWidth=0);
    Sprite(ComPtr<ID2D1Bitmap> image, Size maxFrame, size_t frameEndWidth);

public:
    void SetDuration(float duration);

    void Update(float deltaTime);
    void AdvanceFrame();
    void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarger, D2D1_POINT_2F position);
    void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarger, D2D1_POINT_2F position, float rotAngle);

private:
    ComPtr<ID2D1Bitmap> mImage{ };
    Size mFrameSize{ };
    Size mMaxFrameCount{ };

    unsigned short mFrameCount{ };

    size_t mFrameEnd{ };

    float mCounter{ };
    float mFramePerSecond{ };
    float mDuration{ };
};