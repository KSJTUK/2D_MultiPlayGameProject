#pragma once

#include "Utils.h"

namespace Img {
    inline ComPtr<ID2D1Bitmap> ImageLoad(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const std::wstring_view path)
    {
        HRESULT hr;

        IWICFormatConverter* formatConverter;
        hr = wicFactory->CreateFormatConverter(&formatConverter);
        CheckHR(hr);

        IWICBitmapDecoder* decoder;
        hr = wicFactory->CreateDecoderFromFilename(path.data(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
        CheckHR(hr);

        IWICBitmapFrameDecode* frame;
        hr = decoder->GetFrame(0, &frame);
        CheckHR(hr);

        hr = formatConverter->Initialize(frame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
        CheckHR(hr);

        ComPtr<ID2D1Bitmap> bitmap;
        hr = renderTarget->CreateBitmapFromWicBitmap(
            formatConverter,
            NULL,
            bitmap.GetAddressOf()
        );
        CheckHR(hr);

        decoder->Release();
        frame->Release();
        formatConverter->Release();

        return bitmap;
    }
}