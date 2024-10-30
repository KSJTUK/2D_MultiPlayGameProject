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

    void Render();

private:  
    HINSTANCE mInstance;
    std::unique_ptr<class Window> mMainWindow;

    ComPtr<ID2D1Factory1> mD2Factory; /* Direct2D 자원 객체 생성을 위한 Factory */
    ComPtr<ID2D1HwndRenderTarget> mRenderTarget; /* Window  대상 렌더타겟 */

    ComPtr<IWICImagingFactory> mWICFactory;
    std::unique_ptr<class Image> mTestImage; /* 이미지 로딩 테스트용 */
    std::unique_ptr<class Sprite> mTestSprite; /* 스프라이트 테스트용 */
    std::unique_ptr<class Sprite> mTestSprite2;
};