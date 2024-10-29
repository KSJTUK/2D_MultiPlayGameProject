#include "pch.h"
#include "GameFrame.h"
#include "Window.h"

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
}

GameFrame::~GameFrame()
{
}

void GameFrame::Init()
{
}

void GameFrame::InitDirect2D()
{
}
