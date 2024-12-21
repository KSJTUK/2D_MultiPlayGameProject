#pragma once

inline constexpr float DEFAULT_EFFECT_DUE = 3.0f;

enum FadeType {
    FADE_NONE,
    FADE_IN,
    FADE_OUT,
    COUNT
};

class FadeEffect {
public:
    FadeEffect(FadeType type=FADE_OUT);
    ~FadeEffect();

public:
    void Reset();
    void SetDuration(const float due);
    void SetType(const FadeType type);

public:
    void Update(const float deltaTime);
    void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget);

private:
    FadeType mFadeType{ };
    float mAlpha{ 1.0f };
    float mCount{ };
    float mDuration{ DEFAULT_EFFECT_DUE };
};