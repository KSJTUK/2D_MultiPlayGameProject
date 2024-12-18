#include "pch.h"
#include "GameFrame.h"
#include "Window.h" /* 메인 윈도우 생성 */

#include "Utils.h" /* 에러 및 디버그 용 */
#include "Image.h"
#include "Sprite.h"
#include "Camera.h"
#include "TextWriter.h"
#include "Timer.h"

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
    ImGui_ImplD2D_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
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

void GameFrame::InitImgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::GetStyle().ScaleAllSizes(1);
    io.FontGlobalScale = 1;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
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

void GameFrame::Update() {
    
}

void GameFrame::ImguiRenderStart() {
    // Start the Dear ImGui frame
    ImGui_ImplD2D_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void GameFrame::ImguiUpdateFrame() {
    ImGui::Begin("Hello Imgui");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    ImGui::End();
}

void GameFrame::Render() {
    ImguiRenderStart();

    ImguiUpdateFrame();

    ImGui::EndFrame();

    mRenderTarget->BeginDraw();
    mRenderTarget->Clear(Color(D2D1::ColorF::Gray));
    mRenderTarget->SetTransform(mCamera->GetCameraTransform());

    // Render
    RenderDebugInfo();

    mRenderTarget->SetTransform(Matrix3x2::Identity());

    ImGui::Render();

    ImGui_ImplD2D_RenderDrawData(ImGui::GetDrawData());

    mRenderTarget->EndDraw();
}
