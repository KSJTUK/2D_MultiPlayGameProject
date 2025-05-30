#pragma once

using Size = D2D1_SIZE_U;
using SizeF = D2D1_SIZE_F;
using Rect = D2D1_RECT_U;
using RectF = D2D1_RECT_F;
using RectL = D2D1_RECT_L;

using Point = D2D1_POINT_2L;
using Position = D2D1_POINT_2F;
using Vec2D = D2D1_VECTOR_2F;
using Vec3D = D2D1_VECTOR_3F;

using Matrix3x2 = D2D1::Matrix3x2F;
using Matrix4x3 = D2D1::Matrix4x3F;
using Matrix4x4 = D2D1::Matrix4x4F;

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;