#pragma once

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							GameFrame                                   //
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

class GameFrame {
public:
    GameFrame(HINSTANCE instance);
    ~GameFrame();

public:
    SizeF GetCoordRate() const;
    
public:
    void Init();
    void InitDirect2D();
    void InitWIC();
    void InitCamera();
    void InitImgui();
    void InitText();
    void InitObjects();

    void ResetSize();

    void Update();
    void ImguiRenderStart();

    void PrepareRender();
    void Render();
    void RenderEnd();

private:  
    HINSTANCE mInstance;
    std::unique_ptr<class Window> mMainWindow;
   
    ComPtr<ID2D1Factory1> mD2Factory;                   /* Direct2D 자원 객체 생성을 위한 Factory */
    ComPtr<ID2D1HwndRenderTarget> mRenderTarget;        /* Window  대상 렌더타겟 */
    ComPtr<IWICImagingFactory> mWICFactory;             /* WIC 객체들을 생성할 Factory */

    std::unique_ptr<class TextWriter> mTextWriter;      /* 텍스트 로딩용 클래스*/
    std::unique_ptr<class Camera> mCamera;              /* 카메라 */
    std::unique_ptr<class Timer> mTimer;                /* 타이머 */

    std::unique_ptr<class Sprite> mSprite;

    std::unique_ptr<class TextWindow> mGuiWindow;

    bool mShowDemoWindow{ true };
    bool mShowAnotherWindow{ false };
    ImVec4 mImguiClearColor{ 0.45f, 0.55f, 0.60f, 1.00f };
};