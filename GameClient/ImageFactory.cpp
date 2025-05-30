#include "pch.h"
#include "ImageFactory.h"
#include "Utils.h"

bool ImageFactory::Init(ComPtr<ID2D1Factory1> factory, ComPtr<ID2D1HwndRenderTarget> renderTarget) {
    HRESULT hr;

    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&mWICFactory));
    CheckHR(hr);

    mRenderTarget = renderTarget;

    return true;
}

void ImageFactory::Destroy() {
    mWICFactory->Release();
}

ComPtr<ID2D1Bitmap> ImageFactory::ImageLoad(const std::wstring_view path) {
    HRESULT hr;

    IWICFormatConverter* formatConverter;
    hr = mWICFactory->CreateFormatConverter(&formatConverter);
    CheckHR(hr);

    IWICBitmapDecoder* decoder;
    hr = mWICFactory->CreateDecoderFromFilename(path.data(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
    CheckHR(hr);

    IWICBitmapFrameDecode* frame;
    hr = decoder->GetFrame(0, &frame);
    CheckHR(hr);

    hr = formatConverter->Initialize(frame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
    CheckHR(hr);

    ComPtr<ID2D1Bitmap> bitmap;
    hr = mRenderTarget->CreateBitmapFromWicBitmap(
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
