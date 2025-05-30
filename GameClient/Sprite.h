#pragma once

struct SpriteProperties {
    std::string tag{ };
    std::wstring imagePath{ };
    float duration{ };

    Size frameSize{ };
    Size framePixelSize{ };

    unsigned short maxFrameCount{ };

    SpriteProperties() = delete;
    SpriteProperties(std::string_view tag, const Size& frameSize, const Size& imageSize, unsigned short frameCount = 0);
};

class Sprite {
public:
    Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::string_view path, Size maxFrame, size_t frameEndWidth=0);
    Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::wstring_view path, Size maxFrame, size_t frameEndWidth=0);
    Sprite(ComPtr<ID2D1Bitmap> image, Size maxFrame, size_t frameEndWidth);

public:
    void ChangeDuration(float duration);

    void Update(float deltaTime);
    void AdvanceFrame();
    void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarger, D2D1_POINT_2F position, float rotAngle, float opacity=1.0f);

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