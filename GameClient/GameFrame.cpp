#include "pch.h"
#include "GameFrame.h"
#include "Window.h" /* 메인 윈도우 생성 */

#include "Utils.h" /* 에러 및 디버그 용 */
#include "Image.h"
#include "Sprite.h"
#include "Camera.h"

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
    InitTextWriter();
    InitCamera();
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
}

void GameFrame::InitTextWriter() {
    HRESULT hr;
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory3), reinterpret_cast<IUnknown**>(mWriteFactory.GetAddressOf()));
    CheckHR(hr, std::source_location::current());

    hr = mWriteFactory->CreateTextFormat(
        L"Verdana",                     // 폰트 패밀리 이름의 문자열
        NULL,                           // 폰트 컬렉션 객체, NULL=시스템 폰트 컬렉션
        DWRITE_FONT_WEIGHT_NORMAL,      // 폰트 굵기. LIGHT, NORMAL, BOLD 등.
        DWRITE_FONT_STYLE_NORMAL,       // 폰트 스타일. NORMAL, OBLIQUE, ITALIC.
        DWRITE_FONT_STRETCH_NORMAL,     // 폰트 간격. CONDENSED, NORMAL, MEDIUM, EXTEXDED 등.
        13,                             // 폰트 크기.
        L"ko-kr",                       // 로케일을 문자열로 명시.  영어-미국=L"en-us", 한국어-한국=L"ko-kr"
        reinterpret_cast<IDWriteTextFormat**>(mTextFormat.GetAddressOf())
    );

    CheckHR(hr, std::source_location::current());
    mTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

void GameFrame::InitCamera() {
    mTestCamera = std::make_unique<Camera>();
    mTestCamera->SetViewRange(mRenderTarget);
}

void GameFrame::Render() {
    mRenderTarget->BeginDraw();

    mRenderTarget->Clear(Color(D2D1::ColorF::Gray));

    static ID2D1SolidColorBrush* brush;
    if (nullptr == brush)
        mRenderTarget->CreateSolidColorBrush(Color(D2D1::ColorF::Black), &brush);

    auto [cx, cy] = mRenderTarget->GetSize();
    std::wstring text{ L"안녕하세요." };
    mRenderTarget->DrawTextW(text.c_str(), text.size(), mTextFormat.Get(), D2D1::RectF(0.0f, 0.0f, cx, cy), brush);

    mRenderTarget->EndDraw();
}
