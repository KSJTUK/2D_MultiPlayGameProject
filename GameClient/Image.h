#pragma once

#include "RenderingComponent.h"

class Image : public IRenderingComponent {
public:
    Image(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::wstring_view path);
    Image(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::string_view path);
    Image(ComPtr<ID2D1Bitmap> image);

public:
    virtual void Update(float deltaTime) override;
    virtual void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, D2D1_POINT_2F position, float rotAngle=0.0f, float opacity=1.0f) override;

private:
    ComPtr<ID2D1Bitmap> mImage;
};