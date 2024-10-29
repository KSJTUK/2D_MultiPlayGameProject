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

    void Render();

private:  
    HINSTANCE mInstance;
    std::unique_ptr<class Window> mMainWindow;

    ComPtr<ID2D1Factory1> mD2Factory; /* Direct2D ÀÚ¿ø °´Ã¼ »ý¼ºÀ» À§ÇÑ Factory */
    ComPtr<ID2D1HwndRenderTarget> mRenderTarget; /* Window  ´ë»ó ·»´õÅ¸°Ù */
};