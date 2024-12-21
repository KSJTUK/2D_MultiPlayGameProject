#include "pch.h"
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

    // TEST
    if (mCount >= mDuration) {
        FadeType type = GetCurrentFadeType() == FADE_IN ? FADE_OUT : FADE_IN;
        ChangeEffect(type);
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
    renderTarget->FillRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), brush);
    brush->SetOpacity(1.0f);

    renderTarget->SetTransform(oldTrans);
}
