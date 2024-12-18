#include "pch.h"
#include "GameFrame.h"
#include "Window.h" /* 메인 윈도우 생성 */

#include "Utils.h" /* 에러 및 디버그 용 */
#include "Image.h"
#include "Sprite.h"
#include "Camera.h"
#include "TextWriter.h"

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
    InitCamera();
    InitText();
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

   SolidBrush::Init(mRenderTarget);
}

void GameFrame::InitWIC() {
    HRESULT hr;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    CheckHR(hr, std::source_location::current());

    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(mWICFactory.GetAddressOf()));
    CheckHR(hr, std::source_location::current());
}

void GameFrame::InitCamera() {
    mCamera = std::make_unique<Camera>();
    auto size = SizeFToPosition(mRenderTarget->GetSize());
    mCamera->SetPosition(size * 0.5f);
    mCamera->SetViewRange(mRenderTarget);
}

void GameFrame::InitText() {
    mTextWriter = std::make_unique<TextWriter>();
    mDebugInfo = std::make_unique<DebugInfo>();
}

void GameFrame::RenderDebugInfo() {
    auto [l, t, r, b] = mMainWindow->GetScreenRect();
    l += 100;
    t += 20;
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t) }, to_wstring("HelloWorld"));
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t += 20) }, to_wstring("HelloWorld"), D2D1::ColorF::Black);
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t += 20) }, to_wstring("HelloWorld"), D2D1::ColorF::Cyan);
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t += 20) }, to_wstring("HelloWorld"), D2D1::ColorF::AntiqueWhite);
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t += 20) }, to_wstring("HelloWorld"), D2D1::ColorF::BlanchedAlmond);
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t += 20) }, to_wstring("HelloWorld"), D2D1::ColorF::DeepPink);
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t += 20) }, to_wstring("HelloWorld"), D2D1::ColorF::DarkRed);
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t += 20) }, to_wstring("HelloWorld"), D2D1::ColorF::DeepSkyBlue);
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t += 20) }, to_wstring("HelloWorld"), D2D1::ColorF::BlueViolet);
    mTextWriter->WriteText(mRenderTarget, Position{ static_cast<float>(l), static_cast<float>(t += 20) }, to_wstring("HelloWorld"), D2D1::ColorF::Violet);
}

void GameFrame::Render() {
    mRenderTarget->BeginDraw();

    mRenderTarget->Clear(Color(D2D1::ColorF::White));
    mRenderTarget->SetTransform(mCamera->GetCameraTransform());

    RenderDebugInfo();

    mRenderTarget->EndDraw();
}
