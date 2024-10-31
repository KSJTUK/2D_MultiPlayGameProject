#include "pch.h"
#include "Image.h"
#include "ImageLoader.h"

Image::Image(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
    const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::wstring_view path) {
    mImage = Img::ImageLoad(factory, wicFactory, renderTarget, path);
}

Image::Image(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
    const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::string_view path) {
    mImage = Img::ImageLoad(factory, wicFactory, renderTarget, to_wstring(path));
}

Image::Image(ComPtr<ID2D1Bitmap> image) {
    mImage = image;
}

void Image::Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, D2D1_POINT_2F position) {
    auto [cx, cy] = mImage->GetSize();
    auto source = D2D1::RectF(
        0.0f, 0.0f, cx, cy
    );

    auto dest = D2D1::RectF(
        position.x - cx * 0.5f,
        position.y - cy * 0.5f,
        position.x + cx * 0.5f,
        position.y + cy * 0.5f
    );

    renderTarget->DrawBitmap(mImage.Get(), dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
}

void Image::Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, D2D1_POINT_2F position, float rotAngle) {
    Matrix3x2 oldTrans;
    renderTarget->GetTransform(&oldTrans);
    /* 출력 대상 지점의 중심을 기준으로 회전 */
    renderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(rotAngle, position) * oldTrans);
    Render(renderTarget, position);
    /* 출력 한 후에는 다시 원래의 변환행렬로 변경 */
    renderTarget->SetTransform(oldTrans);
}
