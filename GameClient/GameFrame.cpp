#include "pch.h"
#include "GameFrame.h"
#include "Window.h" /* ���� ������ ���� */

#include "Utils.h" /* ���� �� ����� �� */
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
    InitCamera();
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

void GameFrame::InitWIC() {
    HRESULT hr;
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    CheckHR(hr, std::source_location::current());

    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(mWICFactory.GetAddressOf()));
    CheckHR(hr, std::source_location::current());

    mTestImage = std::make_unique<Image>(mD2Factory, mWICFactory, mRenderTarget, L"Bridge_ALL.png");
    mTestSprite = std::make_unique<Sprite>(mD2Factory, mWICFactory, mRenderTarget, L"Rocks_04.png", D2D1::SizeU(8, 1));
    mTestSprite2 = std::make_unique<Sprite>(mD2Factory, mWICFactory, mRenderTarget, L"Explosions.png", D2D1::SizeU(9, 1));
}

void GameFrame::InitCamera() {
    mTestCamera = std::make_unique<Camera>();
    mTestCamera->SetViewRange(mRenderTarget);
}

void GameFrame::Render() {
    mRenderTarget->BeginDraw();

    mRenderTarget->Clear(Color(D2D1::ColorF::Gray));

    static D2D1_POINT_2F p{ 1.0f, 1.0f };
    static int delayPoint;
    ++delayPoint;
    if (delayPoint > 10) {
        p.x += 1.0f;
        p.y += 1.0f;
        delayPoint = 0;
    }
    /* ��������Ʈ�� ������ ������Ʈ ������ �ڵ� : �׽�Ʈ�뵵�� */
    static int delayFrameCount = 0;
    constexpr int delayFrame = 0;

    /* ������ ���࿡ ���� ȸ���ϴ� �̹����� ���� �� ��� �ڵ� */
    static float delayRoation = 0.0f;
    delayRoation += 0.2f;
    ++delayFrameCount;
    if (delayFrameCount > delayFrame) {
        mTestSprite->AdvanceFrame();
        mTestSprite2->AdvanceFrame();
        delayFrameCount = 0;
    }

    mTestCamera->SetPosition(mTestCamera->GetPosition() + Position{ 0.3f, 0.3f });
    mRenderTarget->SetTransform(mTestCamera->GetCameraTransform());

    mTestImage->Render(mRenderTarget, D2D1::Point2F(500.0f, 500.0f));
    mTestSprite->Render(mRenderTarget, D2D1::Point2F(200.0f, 200.0f), delayRoation);
    mTestSprite2->Render(mRenderTarget, D2D1::Point2F(500.0f, 200.0f), delayRoation);


    mRenderTarget->EndDraw();
}
