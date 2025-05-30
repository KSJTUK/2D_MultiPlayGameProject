#pragma once

class IRenderingComponent abstract {
public:
    IRenderingComponent() { }
    virtual ~IRenderingComponent() { }

public:
    virtual void Update(float deltaTime) abstract;
    virtual void Render(const ComPtr<ID2D1HwndRenderTarget>& renderTarget, Position position, float rotAngle = 0.0f, float opacity = 1.0f) abstract;
};