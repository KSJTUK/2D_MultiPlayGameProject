﻿#include "pch.h"
#include "FadeEffect.h"
#include "GameFrame.h"
#include "Utils.h"

FadeEffect::FadeEffect(FadeType type, float due)
    : mFadeType{ type }, mDuration{ due } {
    mAlpha = FADE_IN == mFadeType ? 1.0f : 0.0f;
}

FadeEffect::~FadeEffect() { }

FadeType FadeEffect::GetCurrentFadeType() const {
    return mFadeType;
}

void FadeEffect::Reset() {
    mCount = 0.0f;
    mAlpha = FADE_IN == mFadeType ? 1.0f : 0.0f;
}

void FadeEffect::ChangeEffect(const FadeType type, float due) {
    mCount = 0.0f;
    mDuration = due;
    mFadeType = type;
    mAlpha = FADE_IN == mFadeType ? 1.0f : 0.0f;
}

void FadeEffect::Update(const float deltaTime) {
    mCount += deltaTime;
    if (mFadeType == FADE_IN) {
        mAlpha = std::clamp(mAlpha - (deltaTime / mDuration), 0.0f, 1.0f);
    }
    else if (mFadeType == FADE_OUT) {
        mAlpha = std::clamp(mAlpha + (deltaTime / mDuration), 0.0f, 1.0f);
    }
}

void FadeEffect::Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget) {
    Matrix3x2 oldTrans{ };
    renderTarget->GetTransform(&oldTrans);
    renderTarget->SetTransform(Matrix3x2::Identity());

    RECT rc{ };
    GetClientRect(renderTarget->GetHwnd(), &rc);
    auto brush = SolidBrush::GetBrush(D2D1::ColorF::Black);
    brush->SetOpacity(mAlpha);

    RectF fRC{ D2D1::RectF(static_cast<float>(rc.left), static_cast<float>(rc.top), static_cast<float>(rc.right), static_cast<float>(rc.bottom)) };
    renderTarget->FillRectangle(fRC, brush);
    brush->SetOpacity(1.0f);

    renderTarget->SetTransform(oldTrans);
}
