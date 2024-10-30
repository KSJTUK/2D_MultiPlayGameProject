#pragma once

class Image {
public:
    Image(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::wstring_view path);
    Image(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, std::string_view path);
    Image(ComPtr<ID2D1Bitmap> image);

public:
    void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, D2D1_POINT_2F position);

private:
    ComPtr<ID2D1Bitmap> mImage;
};