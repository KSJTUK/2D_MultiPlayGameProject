#pragma once

#include "RenderingComponent.h"

class GameObject {
public:
    GameObject();
    GameObject(std::unique_ptr<IRenderingComponent>&& renderer);
    ~GameObject();

public:
    bool IsActive() const;
    float GetRotation() const;
    Position GetPosition() const;

    void SetRotation(float rot);
    void SetPosition(Position pos);

    void AddRot(float rot);
    void AddPos(Position vec);

    void Update(float deltaTime);
    void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget);

private:
    bool mActive{ };
    float mRotation{ 0.0f };
    Position mPosition{ 0.0f, 0.0f };
    std::unique_ptr<IRenderingComponent> mRenderer{ nullptr };
};