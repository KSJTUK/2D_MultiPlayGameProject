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
   
    ComPtr<ID2D1Factory1> mD2Factory;                   /* Direct2D �ڿ� ��ü ������ ���� Factory */
    ComPtr<ID2D1HwndRenderTarget> mRenderTarget;        /* Window  ��� ����Ÿ�� */
    ComPtr<IWICImagingFactory> mWICFactory;             /* WIC ��ü���� ������ Factory */

    std::unique_ptr<class TextWriter> mTextWriter;      /* �ؽ�Ʈ �ε��� Ŭ����*/
    std::unique_ptr<class Camera> mCamera;              /* ī�޶� */
    std::unique_ptr<class Timer> mTimer;                /* Ÿ�̸� */

    std::unique_ptr<class Sprite> mSprite;

    std::unique_ptr<class TextWindow> mGuiWindow;

    bool mShowDemoWindow{ true };
    bool mShowAnotherWindow{ false };
    ImVec4 mImguiClearColor{ 0.45f, 0.55f, 0.60f, 1.00f };
};