#pragma once

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							GameFrame                                   //
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

struct DebugInfo {
    std::string fps{ "FPS: " };
    std::string deltaTime{ "DeltaTime: "};
};

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

    void InitText();

    void RenderDebugInfo();
    void Render();

private:  
    HINSTANCE mInstance;
    std::unique_ptr<class Window> mMainWindow;
   
    ComPtr<ID2D1Factory1> mD2Factory;                   /* Direct2D 자원 객체 생성을 위한 Factory */
    ComPtr<ID2D1HwndRenderTarget> mRenderTarget;        /* Window  대상 렌더타겟 */
    ComPtr<IWICImagingFactory> mWICFactory;             /* WIC 객체들을 생성할 Factory */

    std::unique_ptr<class TextWriter> mTextWriter;      /* 텍스트 로딩용 클래스*/
    std::unique_ptr<class Camera> mCamera;              /* 카메라 */

    std::unique_ptr<DebugInfo> mDebugInfo;

    std::unique_ptr<class Sprite> mSprite;
};