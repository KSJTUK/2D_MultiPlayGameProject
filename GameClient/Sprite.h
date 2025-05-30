#pragma once

#include "RenderingComponent.h"

struct SpriteProperties {
    std::string tag{ };
    float duration{ };

    Size frameSize{ };
    Size framePixelSize{ };

    unsigned short maxFrameCount{ };
    ComPtr<ID2D1Bitmap> image{ nullptr };

    SpriteProperties() = delete;
    SpriteProperties(std::string_view tag, std::wstring_view path, const Size& frameSize, const Size& imageSize, unsigned short frameCount = 0);
};

class Sprite : public IRenderingComponent {
public:
    Sprite(std::string_view path, Size maxFrame, size_t frameEndWidth=0);
    Sprite(std::wstring_view path, Size maxFrame, size_t frameEndWidth=0);
    Sprite(ComPtr<ID2D1Bitmap> image, Size maxFrame, size_t frameEndWidth);
    Sprite(const Sprite& other);

public:
    void ChangeDuration(float duration);
    void AdvanceFrame();

    virtual void Update(float deltaTime) override;
    virtual void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarger, Position position, float rotAngle=0.0f, float opacity=1.0f) override;

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