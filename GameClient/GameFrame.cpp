#include "pch.h"
#include "GameFrame.h"
#include "Window.h" /* 메인 윈도우 생성 */

#include "Utils.h" /* 에러 및 디버그 용 */
#include "Image.h"
#include "Sprite.h"
#include "Camera.h"
#include "TextWriter.h"
#include "Timer.h"

#include "GuiWindow.h"

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
    // Imgui 에서 hwndRenderTarget의 ref count를 여러번 감소시키는 것으로 보임
    // 오류 해결을 위한 임시 조치 -> Imgui destroy이전에 ref count를 알맞게 조정해줌
    mRenderTarget->AddRef();

    ImGui_ImplD2D_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    SolidBrush::Destroy();
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
    InitImgui();
    InitObjects();
}

void GameFrame::InitDirect2D() {
    HRESULT hr;

    /* 멀티 쓰레드 환경에서 동작할 Factory 객체를 만드려면 D2D1_FACTORY_TYPE_MULTI_THREADED 값을 넣어준다. */
#if defined(DEBUG) || defined(_DEBUG)
    D2D1_FACTORY_OPTIONS options;
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        options,
        mD2Factory.GetAddressOf()
    );
#else
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        mD2Factory.GetAddressOf()
    );
#endif

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

    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(mWICFactory.GetAddressOf()));
    CheckHR(hr, std::source_location::current());
}

void GameFrame::InitCamera() {
    mCamera = std::make_unique<Camera>();
    auto size = SizeFToPosition(mRenderTarget->GetSize());
    mCamera->SetPosition(size * 0.5f);
    mCamera->SetViewRange(mRenderTarget);
}

void GameFrame::InitImgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::GetStyle().ScaleAllSizes(1);
    io.FontGlobalScale = 1;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    //window_flags |= ImGuiWindowFlags_NoMove
    ImGui::GetStyle().AntiAliasedFill = false;
    ImGui::GetStyle().AntiAliasedLines = false;
    ImGui::GetStyle().AntiAliasedLinesUseTex = false;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // required styles
    ImGui::GetStyle().AntiAliasedLines = false;

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(mMainWindow->GetHandle());
    ImGui_ImplD2D_Init(mRenderTarget.Get(), mTextWriter->GetWriteFactory().Get());

    mGuiWindow = std::make_unique<GuiWindow>();

    mGuiWindow->EnableWindowFlag(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_UnsavedDocument | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav);
    mGuiWindow->SetWindowSize(ImVec2{ 500, 400 });
    mGuiWindow->SetBgAlpha(0.5f);
}

void GameFrame::InitText() {
    mTextWriter = std::make_unique<TextWriter>();
}

void GameFrame::InitObjects() {
    mTimer = std::make_unique<Timer>();
    mSprite = std::make_unique<Sprite>(mD2Factory, mWICFactory, mRenderTarget, L"Asset/Explosions.png", D2D1::SizeU(9, 1));
    mSprite->SetDuration(1.0f);
}

void GameFrame::ResetSize() {
    auto rc = mMainWindow->GetRect();
    Size resizedSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
    mCamera->SetViewRange(mRenderTarget);
    mRenderTarget->Resize(resizedSize);
}

void GameFrame::Update() {
    mTimer->AdvanceTime();
    const float deltaTime = mTimer->GetDeltaTime<float>();
    mSprite->Update(deltaTime);
}

void GameFrame::ImguiRenderStart() {
    // Start the Dear ImGui frame
    ImGui_ImplD2D_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void GameFrame::PrepareRender() {
    ImguiRenderStart();
    mRenderTarget->BeginDraw();
    mRenderTarget->Clear(Color(D2D1::ColorF::Gray));
    mRenderTarget->SetTransform(mCamera->GetCameraTransform());
}

void GameFrame::Render() {
    PrepareRender();
    // Render
    mSprite->Render(mRenderTarget, Position{ 100, 200 });

    mGuiWindow->Render();

    RenderEnd();
}

void GameFrame::RenderEnd() {
    mRenderTarget->SetTransform(Matrix3x2::Identity());

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplD2D_RenderDrawData(ImGui::GetDrawData());

    mRenderTarget->EndDraw();
}
