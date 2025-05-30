#include "pch.h"
#include "Sprite.h"
#include "ImageLoader.h"

Sprite::Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory, const ComPtr<ID2D1HwndRenderTarget>& renderTarget,
    std::string_view path, Size maxFrame, size_t frameEndWidth) : mFrameCount{ 0 }, mMaxFrameCount{ maxFrame }, mFrameEnd{ frameEndWidth } {
    mImage = Img::ImageLoad(factory, wicFactory, renderTarget, to_wstring(path));

    auto [px, py] = mImage->GetPixelSize();
    mFrameSize = D2D1::SizeU(px / maxFrame.width, py / maxFrame.height);

    if (0 == mFrameEnd) {
        mFrameEnd = mMaxFrameCount.width + mMaxFrameCount.height;
    }
}

Sprite::Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory, const ComPtr<ID2D1HwndRenderTarget>& renderTarget,
    std::wstring_view path, Size maxFrame, size_t frameEndWidth) : mFrameCount{ 0 }, mMaxFrameCount{ maxFrame }, mFrameEnd{ frameEndWidth } {
    mImage = Img::ImageLoad(factory, wicFactory, renderTarget, path);

    auto [px, py] = mImage->GetPixelSize();
    mFrameSize = D2D1::SizeU(px / maxFrame.width, py / maxFrame.height);

    if (0 == mFrameEnd) {
        mFrameEnd = mMaxFrameCount.width + mMaxFrameCount.height;
    }
}

Sprite::Sprite(ComPtr<ID2D1Bitmap> image, Size maxFrame, size_t frameEndWidth) : mFrameCount{ 0 }, mMaxFrameCount{ maxFrame }, mFrameEnd{ frameEndWidth } {
    mImage = image;

    auto [px, py] = mImage->GetPixelSize();
    mFrameSize = D2D1::SizeU(px / maxFrame.width, py / maxFrame.height);

    if (0 == mFrameEnd) {
        mFrameEnd = mMaxFrameCount.width + mMaxFrameCount.height;
    }
}

void Sprite::ChangeDuration(float duration) {
    mDuration = duration;
    mFramePerSecond = mDuration / mFrameEnd;
}

void Sprite::Update(float deltaTime) {
    mCounter += deltaTime;
    if (mCounter >= mFramePerSecond) {
        AdvanceFrame();
        mCounter = 0.0f;
    }
}

void Sprite::AdvanceFrame() {
    ++mFrameCount;

    if (mFrameCount > mFrameEnd - 1) {
        mFrameCount = 0;
    }
}

void Sprite::Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, D2D1_POINT_2F position, float rotAngle, float opacity) {
    /* 그릴 대상의 프레임 정보를 계산 */
    long frameX = mFrameCount % mMaxFrameCount.width;
    long frameY = mFrameCount / mMaxFrameCount.width;
    auto source = D2D1::RectF(
        static_cast<float>(frameX * mFrameSize.width),
        static_cast<float>(frameY * mFrameSize.height),
        static_cast<float>((frameX + 1) * mFrameSize.width),
        static_cast<float>((frameY + 1) * mFrameSize.height)
    );

    auto dest = CreateRectF(position, mFrameSize);

    Matrix3x2 oldTrans;
    renderTarget->GetTransform(&oldTrans);
    /* 출력 대상 지점의 중심을 기준으로 회전 */
    renderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(rotAngle, position) * oldTrans);

    renderTarget->DrawBitmap(mImage.Get(), dest, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
    
    /* 출력 한 후에는 다시 원래의 변환행렬로 변경 */
    renderTarget->SetTransform(oldTrans);
}

SpriteProperties::SpriteProperties(std::string_view tag, const Size& frameSize, const Size& imageSize, unsigned short frameCount)
    : tag{ tag }, frameSize{ frameSize }, maxFrameCount{ frameCount } {
    framePixelSize = D2D1::Size(imageSize.width / frameSize.width, imageSize.height / frameSize.height);
    if (0 == maxFrameCount) {
        maxFrameCount = frameSize.width + frameSize.height;
    }
}
