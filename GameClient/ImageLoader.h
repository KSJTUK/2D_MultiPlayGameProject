#pragma once

#include "Utils.h"

namespace Image {
    inline ComPtr<ID2D1Bitmap> ImageLoad(const ComPtr<ID2D1Factory>& factory, const ComPtr<IWICImagingFactory>& wicFactory,
        const ComPtr<ID2D1HwndRenderTarget>& renderTarget, const std::wstring_view path)
    {
        /* 32bppRGBA 형식으로 변환해줄 Converter 생성, 한번만 생성한다. */
        static ComPtr<IWICFormatConverter> formatConverter;
        if (nullptr == formatConverter) CheckHR(wicFactory->CreateFormatConverter(&formatConverter), std::source_location::current());

        HRESULT hr;

        IWICBitmapDecoder* decoder;
        hr = wicFactory->CreateDecoderFromFilename(path.data(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
        CheckHR(hr, std::source_location::current());

        IWICBitmapFrameDecode* frame;
        hr = decoder->GetFrame(0, &frame);
        CheckHR(hr, std::source_location::current());

        hr = formatConverter->Initialize(frame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
        CheckHR(hr, std::source_location::current());

        ComPtr<ID2D1Bitmap> bitmap;
        hr = renderTarget->CreateBitmapFromWicBitmap(
            formatConverter.Get(),
            NULL,
            bitmap.GetAddressOf()
        );
        CheckHR(hr, std::source_location::current());

        decoder->Release();
        frame->Release();

        return bitmap;
    }
}