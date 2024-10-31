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

    void SetPosition(const Position& position);

    void SetViewRange(const ComPtr<ID2D1HwndRenderTarget>& renderTarget);

#ifndef UNUSE_CAMERA_ROTATION
    float GetRotationAngle() const;
    void SetRotationAngle(float angle);
#endif

private:
#ifndef UNUSE_CAMERA_ROTATION
    /* 카메라 회전 각도 : 2D이므로 roll 회전만 존재한다. */
    float mRotAngle;
#endif
    /* 카메라 위치 */
    Position mPosition; /* center */
    Size mViewRange;    /* size */
};