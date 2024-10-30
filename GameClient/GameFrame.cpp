#include "pch.h"
#include "GameFrame.h"
#include "Window.h" /* 메인 윈도우 생성 */

#include "Utils.h" /* 에러 및 디버그 용 */
#include "ImageLoader.h"

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							GameFrame                                   //
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

GameFrame::GameFrame(HINSTANCE instance) :
    mInstance{ instance },
    mMainWindow{ std::make_unique<Window>(instance, L"2D Game Project") } {

    Init();
}

GameFrame::~GameFrame() {
}

SizeF GameFrame::GetCoordRate() const {
    RECT rc;
    GetClientRect(mMainWindow->GetHandle(), &rc);

    SizeF rtSize = mRenderTarget->GetSize();
    Size clientSize = D2D1::SizeU(rc.right, rc.bottom);

    return SizeF{ rtSize.width / clientSize.width, rtSize.height / clientSize.height };
}

void GameFrame::Init() {
    InitDirect2D();
    InitWIC();
}

void GameFrame::InitDirect2D() {
    HRESULT hr;

    /* 멀티 쓰레드 환경에서 동작할 Factory 객체를 만드려면 D2D1_FACTORY_TYPE_MULTI_THREADED 값을 넣어준다. */
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, mD2Factory.GetAddressOf());
    CheckHR(hr, std::source_location::current());

    auto rc = mMainWindow->GetRect();
    hr = mD2Factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(mMainWindow->GetHandle(), D2D1::SizeU(rc.right, rc.bottom)),
        mRenderTarget.GetAddressOf()
   );
   CheckHR(hr, std::source_location::current());
}

void GameFrame::InitWIC() {
    HRESULT hr;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    CheckHR(hr, std::source_location::current());

    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(mWICFactory.GetAddressOf()));
    CheckHR(hr, std::source_location::current());

    mTestBitmap = Image::ImageLoad(mD2Factory, mWICFactory, mRenderTarget, L"Bridge_ALL.png");
}

void GameFrame::Render() {
    mRenderTarget->BeginDraw();

    mRenderTarget->Clear(Color(D2D1::ColorF::Gray));

    ID2D1SolidColorBrush* pBrush;
    mRenderTarget->CreateSolidColorBrush(Color(D2D1::ColorF::Red), &pBrush);
    D2D1_ELLIPSE ellipse{ D2D1::Point2F(100.0f, 100.0f), 100.0f, 100.0f };
    mRenderTarget->FillEllipse(ellipse, pBrush);
    pBrush->Release();

    RectF rc = D2D1::RectF(200.0f, 200.0f, 200.0f + mTestBitmap->GetSize().width, 200.0f + mTestBitmap->GetSize().height);
    mRenderTarget->DrawBitmap(mTestBitmap.Get(), rc);

    mRenderTarget->EndDraw();
}
