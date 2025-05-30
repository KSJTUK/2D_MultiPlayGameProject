#include "pch.h"
#include "Camera.h"

Position Camera::GetPosition() const {
    return mPosition;
}

Matrix3x2 Camera::GetCameraTransform() const {
    return Matrix3x2::Translation(mPosition.x, mPosition.y);
}

Size Camera::GetViewRange() const {
    return mViewRange;
}

void Camera::SetPosition(const Position& position) {
    mPosition = position;
}

void Camera::SetViewRange(const ComPtr<ID2D1HwndRenderTarget>& renderTarget) {
    mViewRange = renderTarget->GetPixelSize();
}

void Camera::ViewMatrix(const ComPtr<ID2D1HwndRenderTarget>& renderTarget) {
    D2D1::Matrix3x2F m{ };
    m.TransformPoint(mPosition);
    renderTarget->SetTransform(m);
}
