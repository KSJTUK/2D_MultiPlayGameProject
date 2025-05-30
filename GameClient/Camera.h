#pragma once

#define UNUSE_CAMERA_ROTATION

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//							   Camera                                   //
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

const SizeF MIN_VIEW_PORT_SIZE = SizeF{ 640.0f, 360.0f };
const SizeF MONITOR_ASPECT = SizeF{ 16.0f, 9.0f };

class Camera {
public:
    Position GetPosition() const;
    Matrix3x2 GetCameraTransform() const;
    Size GetMainWindowSize() const;
    SizeF GetViewRange() const;
    SizeF GetCameraScale() const;

    void SetPosition(const Position& position);
    void SetMainWindowSize(const ComPtr<ID2D1HwndRenderTarget>& renderTarget);
    void ResetViewRange(SizeF viewPortSize, float maxMapAreaWidth);
    void SetFollowTarget(const Position& target, float speed);

    void SmoothFollow(float deltaTime);
    Position WorldPosToScreen(const Position& pos);
    Position ScreenPosToWorld(const Position& pos);

    void Zoom(float zoom, float maxMapAreaWidth);
    void Clamp(Position ltMap, Position rbMap);

    void Update(float deltaTime);
    void UpdateViewMatrix(const ComPtr<ID2D1HwndRenderTarget>& renderTarget);

private:
    /* 카메라 위치 */
    Position mPosition; /* center */
    Size mMainWindowSize;    /* size */
    SizeF mViewRange{ DEFAULT_WINDOW_SIZE.width * 0.5f, DEFAULT_WINDOW_SIZE.height * 0.5f };    /* size */
    SizeF mScreenScale;

    Position mFollowTarget{ mPosition };
    float mFollowSpeed{ 0.0f };
};