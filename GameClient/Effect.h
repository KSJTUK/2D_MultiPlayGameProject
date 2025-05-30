#pragma once

enum class EffectTag {
    FADE,
};

class Effect abstract {
    inline static constexpr float DEFAULT_EFFECT_DUE = 3.0f;

public:
    virtual void Update(const float deltaTime) abstract;
    virtual void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget) abstract;

private:
    float mTimeCount{ 0.0f };
    float mDuration{ DEFAULT_EFFECT_DUE };
    size_t mLoopCount{ 0 };
    bool mDone{ false };
};

class EffectManager {
public:
    EffectManager();
    ~EffectManager();

public:
    template <typename... Args>
    void EmplaceEffect();

private:
    std::unordered_map<EffectTag, std::vector<std::unique_ptr<Effect>>> mEffectMap;
};