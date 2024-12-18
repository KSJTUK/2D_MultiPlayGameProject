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
   
    ComPtr<ID2D1Factory1> mD2Factory;                   /* Direct2D �ڿ� ��ü ������ ���� Factory */
    ComPtr<ID2D1HwndRenderTarget> mRenderTarget;        /* Window  ��� ����Ÿ�� */
    ComPtr<IWICImagingFactory> mWICFactory;             /* WIC ��ü���� ������ Factory */

    std::unique_ptr<class TextWriter> mTextWriter;      /* �ؽ�Ʈ �ε��� Ŭ����*/
    std::unique_ptr<class Camera> mCamera;              /* ī�޶� */

    std::unique_ptr<DebugInfo> mDebugInfo;

    std::unique_ptr<class Sprite> mSprite;
};