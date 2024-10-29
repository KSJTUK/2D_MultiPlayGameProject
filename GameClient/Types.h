#pragma once

using Size = D2D1_SIZE_U;
using SizeF = D2D1_SIZE_F;
using Rect = D2D1_RECT_U;
using RectF = D2D1_RECT_F;
using RectL = D2D1_RECT_L;

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;