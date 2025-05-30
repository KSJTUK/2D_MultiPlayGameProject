#pragma once

#include "RenderingComponent.h"

class Image : public IRenderingComponent {
public:
    Image(std::wstring_view path);
    Image(std::string_view path);
    Image(ComPtr<ID2D1Bitmap> image);

public:
    virtual void Update(float deltaTime) override;
    virtual void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, Position position, float rotAngle=0.0f, float opacity=1.0f) override;

private:
    ComPtr<ID2D1Bitmap> mImage;
};