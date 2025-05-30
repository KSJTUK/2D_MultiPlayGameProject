#include "pch.h"
#include "Camera.h"
#include "Utils.h"

Position Camera::GetPosition() const {
    return mPosition;
}

Matrix3x2 Camera::GetCameraTransform() const {
    return Matrix3x2::Translation(mPosition.x, mPosition.y);
}

Size Camera::GetMainWindowSize() const {
    return mMainWindowSize;
}

SizeF Camera::GetCameraScale() const {
    return mScreenScale;
}

SizeF Camera::GetViewRange() const {
    return mViewRange;
}

void Camera::SetPosition(const Position& position) {
    mPosition = position;
}

void Camera::SetMainWindowSize(const ComPtr<ID2D1HwndRenderTarget>& renderTarget) {
    mMainWindowSize = renderTarget->GetPixelSize();
    mScreenScale = SizeF{ mMainWindowSize.width / static_cast<float>(mViewRange.width), mMainWindowSize.height / static_cast<float>(mViewRange.height) };
}

void Camera::ResetViewRange(SizeF viewPortSize, float maxMapAreaWidth) {
    // 설정한 모니터 비율로 맞추기
    if (viewPortSize.width * MONITOR_ASPECT.height != viewPortSize.height * MONITOR_ASPECT.width) {
        viewPortSize.height = viewPortSize.width / MONITOR_ASPECT.width * MONITOR_ASPECT.height;
    }

    SizeF scale = SizeF{ mMainWindowSize.width / viewPortSize.width, mMainWindowSize.height / viewPortSize.height };
    if (viewPortSize.width > maxMapAreaWidth * scale.width) {
        maxMapAreaWidth *= scale.width;
        mViewRange = D2D1::SizeF(static_cast<float>(maxMapAreaWidth), static_cast<float>(maxMapAreaWidth / MONITOR_ASPECT.width * MONITOR_ASPECT.height));
        mScreenScale = SizeF{ mMainWindowSize.width / mViewRange.width, mMainWindowSize.height / mViewRange.height };
        return;
    }

    if (viewPortSize.width < MIN_VIEW_PORT_SIZE.width) {
        mViewRange = MIN_VIEW_PORT_SIZE;
        mScreenScale = SizeF{ mMainWindowSize.width / mViewRange.width, mMainWindowSize.height / mViewRange.height };
        return;
    }

    mViewRange = viewPortSize;
    mScreenScale = SizeF{ mMainWindowSize.width / mViewRange.width, mMainWindowSize.height / mViewRange.height };
}

void Camera::SetFollowTarget(const Position& target, float speed) {
    mFollowTarget = target;
    mFollowSpeed = speed;
}

void Camera::SmoothFollow(float deltaTime) {
    float t = 1.0f - std::exp(-mFollowSpeed * deltaTime);
    mPosition.x = std::lerp(mPosition.x, mFollowTarget.x, t);
    mPosition.y = std::lerp(mPosition.y, mFollowTarget.y, t);

    auto dist = Position::DistSq(mPosition, mFollowTarget);
    constexpr float epsilonSq = 0.01f;
    if (dist < epsilonSq) {
        mPosition = mFollowTarget;
        mFollowSpeed = 0.0f;
        mFollowTarget = Position{ };
    }
}

Position Camera::WorldPosToScreen(const Position& pos) {
    Position worldPos{
        (pos.x - mPosition.x + mViewRange.width * 0.5f) * mScreenScale.width,
        (pos.y - mPosition.y + mViewRange.height * 0.5f) * mScreenScale.height
    };

    return worldPos;
}

Position Camera::ScreenPosToWorld(const Position& pos) {
    Position screenPos{
        (pos.x / mScreenScale.width) + mPosition.x - mViewRange.width * 0.5f,
        (pos.y / mScreenScale.height) + mPosition.y - mViewRange.height * 0.5f
    };

    return screenPos;
}

void Camera::Zoom(float zoom, float maxMapAreaWidth) {
    ResetViewRange(mViewRange * zoom, maxMapAreaWidth);
}

void Camera::Clamp(Position ltMap, Position rbMap) {
    SizeF halfView = mViewRange * 0.5f;

    auto clampX = std::clamp(mPosition.x, ltMap.x + halfView.width, rbMap.x - halfView.width);
    auto clampY = std::clamp(mPosition.y, ltMap.y + halfView.height, rbMap.y - halfView.height);

    mPosition = Position{ clampX, clampY };
    if (clampX != mPosition.x) {
        mFollowTarget.x = mPosition.x;
    }

    if (clampY != mPosition.y) {
        mFollowTarget.y = mPosition.y;
    }
}

void Camera::Update(float deltaTime) {
    if (mFollowSpeed > std::numeric_limits<float>::epsilon()) {
        SmoothFollow(deltaTime);
    }
}

void Camera::UpdateViewMatrix(const ComPtr<ID2D1HwndRenderTarget>& renderTarget) {
    auto m = Matrix3x2::Translation(-mPosition.x + mViewRange.width * 0.5f, -mPosition.y + mViewRange.height * 0.5f) * Matrix3x2::Scale(mScreenScale);
    renderTarget->SetTransform(m);
}
