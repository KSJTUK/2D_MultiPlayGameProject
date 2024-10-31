#include "pch.h"
#include "Sprite.h"
#include "ImageLoader.h"

Sprite::Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory, const ComPtr<ID2D1HwndRenderTarget>& renderTarget,
    std::string_view path, Size maxFrame) : mFrameCount{ 0 }, mMaxFrameCount{ maxFrame } {
    mImage = Img::ImageLoad(factory, wicFactory, renderTarget, to_wstring(path));

    auto [px, py] = mImage->GetPixelSize();
    mFrameSize = D2D1::SizeU(px / maxFrame.width, py / maxFrame.height);
}

Sprite::Sprite(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory, const ComPtr<ID2D1HwndRenderTarget>& renderTarget,
    std::wstring_view path, Size maxFrame) : mFrameCount{ 0 }, mMaxFrameCount{ maxFrame } {
    mImage = Img::ImageLoad(factory, wicFactory, renderTarget, path);

    auto [px, py] = mImage->GetPixelSize();
    mFrameSize = D2D1::SizeU(px / maxFrame.width, py / maxFrame.height);
}

Sprite::Sprite(ComPtr<ID2D1Bitmap> image, Size maxFrame) : mFrameCount{ 0 }, mMaxFrameCount{ maxFrame } {
    mImage = image;

    auto [px, py] = mImage->GetPixelSize();
    mFrameSize = D2D1::SizeU(px / maxFrame.width, py / maxFrame.height);
}

void Sprite::AdvanceFrame() {
    ++mFrameCount;

    if (mFrameCount > mMaxFrameCount.width * mMaxFrameCount.height - 1) {
        mFrameCount = 0;
    }
}

void Sprite::Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, D2D1_POINT_2F position) {
    long frameX = mFrameCount % mMaxFrameCount.width;
    long frameY = mFrameCount / mMaxFrameCount.width;

    auto source = D2D1::RectF(
        static_cast<float>(frameX * mFrameSize.width),
        static_cast<float>(frameY * mFrameSize.height),
        static_cast<float>((frameX + 1) * mFrameSize.width),
        static_cast<float>((frameY + 1) * mFrameSize.height)
    );

    auto dest = D2D1::RectF(
        position.x - mFrameSize.width * 0.5f,
        position.y - mFrameSize.height * 0.5f,
        position.x + mFrameSize.width * 0.5f,
        position.y + mFrameSize.height * 0.5f);

    renderTarget->DrawBitmap(mImage.Get(), dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
}

void Sprite::Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, D2D1_POINT_2F position, float rotAngle) {
    Matrix3x2 oldTrans;
    renderTarget->GetTransform(&oldTrans);
    /* 출력 대상 지점의 중심을 기준으로 회전 */
    renderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(rotAngle, position) * oldTrans);
    Render(renderTarget, position);
    /* 출력 한 후에는 다시 원래의 변환행렬로 변경 */
    renderTarget->SetTransform(oldTrans);
}
