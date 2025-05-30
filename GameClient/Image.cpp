#include "pch.h"
#include "Image.h"
#include "ImageFactory.h"
#include "Utils.h"

Image::Image(std::wstring_view path) {
    mImage = ImageFactory::ImageLoad(path);
}

Image::Image(std::string_view path) {
    mImage = ImageFactory::ImageLoad(to_wstring(path));
}

Image::Image(ComPtr<ID2D1Bitmap> image) {
    mImage = image;
}

void Image::Update(float deltaTime) { }

void Image::Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, Position position, float rotAngle, float opacity) {
    Matrix3x2 oldTrans;
    renderTarget->GetTransform(&oldTrans);
    /* 출력 대상 지점의 중심을 기준으로 회전 */
    renderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(rotAngle, position) * oldTrans);

    // Rendering
    SizeF size = mImage->GetSize();
    auto source = D2D1::RectF(
        0.0f, 0.0f, size.width, size.height
    );

    auto dest = CreateRectF(position, size);

    renderTarget->DrawBitmap(mImage.Get(), dest, opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);

    /* 출력 한 후에는 다시 원래의 변환행렬로 변경 */
    renderTarget->SetTransform(oldTrans);
}
