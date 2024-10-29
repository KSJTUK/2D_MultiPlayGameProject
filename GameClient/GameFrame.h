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
    void Init();
    void InitDirect2D();

private:  
    HINSTANCE mInstance;
    std::unique_ptr<class Window> mMainWindow;
};