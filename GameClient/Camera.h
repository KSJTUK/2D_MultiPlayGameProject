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
    /* ī�޶� ȸ�� ���� : 2D�̹Ƿ� roll ȸ���� �����Ѵ�. */
    float mRotAngle;
#endif
    /* ī�޶� ��ġ */
    Position mPosition; /* center */
    Size mViewRange;    /* size */
};