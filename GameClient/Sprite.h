#pragma once

struct SpriteInfo {
    std::string tag{ };
    std::wstring imagePath{ };
    float duration{ };

    Size frameSize{ };
    Size framePixelSize{ };

    unsigned short maxFrameCount{ };

    SpriteInfo() = delete;
    SpriteInfo(std::string_view tag, const Size& frameSize, const Size& imageSize, unsigned short frameCount = 0)
        : tag{ tag }, frameSize{ frameSize }, maxFrameCount{ frameCount } {
        framePixelSize = D2D1::Size(imageSize.width / frameSize.width, imageSize.height / frameSize.height);
        if (0 == maxFrameCount) {
            maxFrameCount = frameSize.width + frameSize.height;
        }
    }
};

class Sprite {
public:
    Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::string_view path, Size maxFrame, size_t frameEndWidth=0);
    Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::wstring_view path, Size maxFrame, size_t frameEndWidth=0);
    Sprite(ComPtr<ID2D1Bitmap> image, Size maxFrame, size_t frameEndWidth);

public:
    void SetDuration(float duration);
    void SetOpacity(float opacity);

    void Update(float deltaTime);
    void AdvanceFrame();
    void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarger, D2D1_POINT_2F position);
    void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarger, D2D1_POINT_2F position, float rotAngle);

private:
    ComPtr<ID2D1Bitmap> mImage{ };
    float mOpacity{ 1.0f };
    Size mFrameSize{ };
    Size mMaxFrameCount{ };

    unsigned short mFrameCount{ };

    size_t mFrameEnd{ };

    float mCounter{ };
    float mFramePerSecond{ };
    float mDuration{ };
};