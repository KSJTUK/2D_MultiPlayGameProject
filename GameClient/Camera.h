#pragma once

#define UNUSE_CAMERA_ROTATION

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							   Camera                                   //
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

class Camera {
public:
    Position GetPosition() const;
    Matrix3x2 GetCameraTransform() const;
    Size GetViewRange() const;

    void SetPosition(const Position& position);
    void SetViewRange(const ComPtr<ID2D1HwndRenderTarget>& renderTarget);

    void Clamp(Position ltMap, Position rbMap);

    void ViewMatrix(const ComPtr<ID2D1HwndRenderTarget>& renderTarget);

private:
    /* 카메라 위치 */
    Position mPosition; /* center */
    Size mViewRange;    /* size */
};