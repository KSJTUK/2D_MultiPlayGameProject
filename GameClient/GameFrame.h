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
    void InitTextWriter();
    void InitCamera();

    void Render();

private:  
    HINSTANCE mInstance;
    std::unique_ptr<class Window> mMainWindow;

    ComPtr<ID2D1Factory1> mD2Factory;                   /* Direct2D �ڿ� ��ü ������ ���� Factory */
    ComPtr<ID2D1HwndRenderTarget> mRenderTarget;        /* Window  ��� ����Ÿ�� */
    ComPtr<IDWriteFactory3> mWriteFactory;              /* Direct Write ���� ��ü ������ ���� Factory */
    ComPtr<IDWriteTextFormat3> mTextFormat;             /* Text ���� */

    ComPtr<IWICImagingFactory> mWICFactory;
    std::unique_ptr<class Camera> mTestCamera;          /* ī�޶� �׽�Ʈ�� */
    std::unique_ptr<class Image> mTestImage;            /* �̹��� �ε� �׽�Ʈ�� */
    std::unique_ptr<class Sprite> mTestSprite;          /* ��������Ʈ �׽�Ʈ�� */
    std::unique_ptr<class Sprite> mTestSprite2;
};