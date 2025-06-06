﻿#include "pch.h"
#include "GameFrame.h"
#include "Window.h" /* 메인 윈도우 생성 */

#include "Utils.h" /* 에러 및 디버그 용 */
#include "Image.h"
#include "Sprite.h"
#include "Camera.h"
#include "TextWriter.h"
#include "Timer.h"

#include "GuiWindow.h"
#include "ImageFactory.h"

#include "FadeEffect.h"

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
    ImageFactory::Destroy();
}

SizeF GameFrame::GetCoordRate() const {
    RECT rc;
    GetClientRect(mMainWindow->GetHandle(), &rc);

    SizeF rtSize = mRenderTarget->GetSize();
    Size clientSize = D2D1::SizeU(rc.right, rc.bottom);

    return SizeF{ rtSize.width / clientSize.width, rtSize.height / clientSize.height };
}

std::unique_ptr<class Camera>& GameFrame::GetMainCamera() {
    return mCamera;
}

void GameFrame::Init() {
    InitDirect2D();
    InitCamera();
    InitText();
    InitImgui();
    InitObjects();

    ResetSize();
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

    CheckHR(hr);

    auto rc = mMainWindow->GetRect();
    hr = mD2Factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(mMainWindow->GetHandle(), D2D1::SizeU(rc.right, rc.bottom), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
        mRenderTarget.GetAddressOf()
   );
   CheckHR(hr);

   SolidBrush::Init(mRenderTarget);

   ImageFactory::Init(mD2Factory, mRenderTarget);
}

void GameFrame::InitCamera() {
    mCamera = std::make_unique<Camera>();
    auto size = SizeFToPosition(mRenderTarget->GetSize());

    mCamera->SetMainWindowSize(mRenderTarget);
}

void GameFrame::InitImgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::GetStyle().ScaleAllSizes(1);
    io.FontGlobalScale = 1;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 14.0f, nullptr, io.Fonts->GetGlyphRangesKorean());

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    //window_flags |= ImGuiWindowFlags_NoMove
    ImGui::GetStyle().AntiAliasedFill = false;
    ImGui::GetStyle().AntiAliasedLines = false;
    ImGui::GetStyle().AntiAliasedLinesUseTex = false;

    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();
    // required styles
    ImGui::GetStyle().AntiAliasedLines = false;

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(mMainWindow->GetHandle());
    ImGui_ImplD2D_Init(mRenderTarget.Get(), mTextWriter->GetWriteFactory().Get());

    mGuiWindow = std::make_unique<ChatWindow>();

    mGuiWindow->EnableWindowFlag(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_UnsavedDocument | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav);
    mGuiWindow->SetWindowSize(ImVec2{ 500, 400 });
    mGuiWindow->SetBgAlpha(0.5f);
}

void GameFrame::InitText() {
    mTextWriter = std::make_unique<TextWriter>();

    FontProperties fontProp;

    fontProp.fontFamilyName = L"Chainsaw Carnage";
    fontProp.size = 30.0f;
    fontProp.locale = L"en-US";
    mTextWriter->LoadFontFromFile(1, "Asset/ChainsawCarnage.ttf", fontProp);
}

void GameFrame::InitObjects() {
    Timer::AddEvent(
        1s,
        [this]() { 
            static auto prevCount = Timer::GetFrameCount();
            mGuiWindow->InputText(L"FPS: "s + std::to_wstring(Timer::GetFrameCount() - prevCount));
            prevCount = Timer::GetFrameCount();
            return true; 
        }
    );

    mTestMapImage = std::make_unique<Image>(std::wstring_view{ L"Asset/samplemap.png" });

    mSprite = std::make_unique<Sprite>(L"Asset/Explosions.png", D2D1::SizeU(9, 1));
    mSprite->ChangeDuration(1.0f);
}

void GameFrame::ResetSize() {
    auto rc = mMainWindow->GetRect();
    Size resizedSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
    mRenderTarget->Resize(resizedSize);
    mCamera->SetMainWindowSize(mRenderTarget);
    mGuiWindow->SetMainWindowSize(mCamera->GetMainWindowSize());
}

void GameFrame::Update() {
    Timer::AdvanceTime();
    const float deltaTime = Timer::GetDeltaTime<float>();
    mSprite->Update(deltaTime);

    mCamera->Update(Timer::GetDeltaTime());
    mCamera->Clamp(TEMP_MAP_LT, TEMP_MAP_RB);
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

    mCamera->UpdateViewMatrix(mRenderTarget);
}

void GameFrame::Render() {
    PrepareRender();

    mTestMapImage->Render(mRenderTarget, CENTER_OF_MAP);

    static Position spritePos = CENTER_OF_MAP;
    static auto rotAngle = Timer::GetDeltaTime();
    rotAngle += Timer::GetDeltaTime() * 200.0f;

    spritePos += Position(0.01f, 0.01f);
    mSprite->Render(mRenderTarget, spritePos, rotAngle);

    mTextWriter->SetFont(DEFAULT_FONT_KEY);
    mTextWriter->WriteText(mRenderTarget, CENTER_OF_MAP, L"문자 출력 테스트");

    mTextWriter->SetFont(1);
    mTextWriter->WriteColorText(mRenderTarget, Position{ 0.0f, 40.0f }, L"Test [ChainsawCarnage.ttf]", D2D1::ColorF::Red);

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
