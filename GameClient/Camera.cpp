#include "pch.h"
#include "Camera.h"

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							   Camera                                   //
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

Position Camera::GetPosition() const {
    return mPosition;
}

Matrix3x2 Camera::GetCameraTransform() const {
    return Matrix3x2::Translation(mPosition.x, mPosition.y);
}

void Camera::SetPosition(const Position& position) {
    mPosition = position;
}

void Camera::SetViewRange(const ComPtr<ID2D1HwndRenderTarget>& renderTarget) {
    mViewRange = renderTarget->GetPixelSize();
}
