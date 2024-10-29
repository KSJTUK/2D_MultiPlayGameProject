#include "pch.h"
#include "GameFrame.h"
#include "Window.h" /* ���� ������ ���� */

#include "Utils.h" /* ���� �� ����� �� */

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
}

void GameFrame::InitDirect2D() {
    HRESULT hr;

    /* ��Ƽ ������ ȯ�濡�� ������ Factory ��ü�� ������� D2D1_FACTORY_TYPE_MULTI_THREADED ���� �־��ش�. */
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

void GameFrame::Render() {
    mRenderTarget->BeginDraw();

    mRenderTarget->Clear(Color(D2D1::ColorF::Gray));

    ID2D1SolidColorBrush* pBrush;
    mRenderTarget->CreateSolidColorBrush(Color(D2D1::ColorF::Red), &pBrush);
    D2D1_ELLIPSE ellipse{ D2D1::Point2F(100.0f, 100.0f), 100.0f, 100.0f };
    mRenderTarget->FillEllipse(ellipse, pBrush);
    pBrush->Release();

    mRenderTarget->EndDraw();
}
