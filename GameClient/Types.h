#pragma once

using Size = D2D1_SIZE_U;
using SizeF = D2D1_SIZE_F;
using Rect = D2D1_RECT_U;
using RectF = D2D1_RECT_F;
using RectL = D2D1_RECT_L;

using Point = D2D1_POINT_2L;
using Vec2D = D2D1_VECTOR_2F;
using Vec3D = D2D1_VECTOR_3F;

using Matrix3x2 = D2D1::Matrix3x2F;
using Matrix4x3 = D2D1::Matrix4x3F;
using Matrix4x4 = D2D1::Matrix4x4F;

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

struct Position : public D2D1_POINT_2F {
    constexpr Position() : D2D1_POINT_2F{ 0.0f, 0.0f } {}
    constexpr Position(float x, float y) : D2D1_POINT_2F{ x, y } { }

    Position(const Position& other) : D2D1_POINT_2F{ other.x, other.y } {}
    Position(Position&& other) noexcept : D2D1_POINT_2F{ other.x, other.y } {}
    Position& operator=(const Position& other) { x = other.x; y = other.y; return *this; }
    Position& operator=(Position&& other) noexcept { x = other.x; y = other.y; return *this; }

    Position operator+(const Position& other) const { return Position{ x + other.x, y + other.y }; }
    void operator+=(const Position& other) { x += other.x; y += other.y; }

    Position operator-(const Position& other) const { return Position{ x - other.x, y - other.y }; }
    void operator-=(const Position& other) { x -= other.x; y -= other.y; }

    Position& operator=(const D2D1_POINT_2F& other) { x = other.x; y = other.y; return *this; }
    Position& operator=(D2D1_POINT_2F&& other) noexcept { x = other.x; y = other.y; return *this; }

    Position operator+(const D2D1_POINT_2F& other) const { return Position{ x + other.x, y + other.y }; }
    void operator+=(const D2D1_POINT_2F& other) { x += other.x; y += other.y; }

    Position operator-(const D2D1_POINT_2F& other) const { return Position{ x - other.x, y - other.y }; }
    void operator-=(const D2D1_POINT_2F& other) { x -= other.x; y -= other.y; }

    Position operator*(float scalar) const { return Position{ x * scalar, y * scalar }; }
    void operator*=(float scalar) { x *= scalar; y *= scalar; }

    Position operator-() const { return Position{ -x, -y }; }

    friend Position operator*(float scalar, const Position& pos) { return Position{ pos.x * scalar, pos.y * scalar }; }

    float LengthSq() const {
        return x * x + y * y;
    }

    float Length() const {
        return std::sqrtf(LengthSq());
    }

    float Dot(const Position& other) {
        return other.x * x + other.y * y;
    }

    static float Dot(const Position& pos1, const Position& pos2) {
        return pos1.x * pos2.x + pos1.y * pos2.y;
    }

    operator D2D1_POINT_2F() {
        return D2D1_POINT_2F{ x, y };
    }
};