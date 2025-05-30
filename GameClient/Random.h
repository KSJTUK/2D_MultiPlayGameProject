#pragma once

class Random {
private:
    template <typename T> requires std::is_integral_v<T>
    using uniform_int = std::uniform_int_distribution<T>;
    
    template <typename T> requires std::is_floating_point_v<T>
    using uniform_real = std::uniform_real_distribution<T>;

    template <typename T> requires std::is_floating_point_v<T>
    using normal = std::normal_distribution<T>;

    inline static std::random_device rd{ };
    inline static std::default_random_engine dre{ rd() };

public:
    template <typename T> requires std::is_arithmetic_v<T>
    static T UniformRand(const T min, const T max) {
        if constexpr (std::is_integral_v<T>) {
            uniform_int<T> uid{ min, max };
            return uid(dre);
        }
        else {
            uniform_real<T> urd{ min, max };
            return urd(dre);
        }
    }

    template <typename T=float> requires std::is_floating_point_v<T>
    static T NormalRand(const T mean, const T sigma = T{ }) {
        normal<T> n{ mean, sigma };
        return n(dre);
    }

    static Position RandPos(Position min, Position max) {
        return Position{ UniformRand(min.x, max.x), UniformRand(min.y, max.y) };
    }
    
    static Vec2D RandVec2D(const Vec2D& min, const Vec2D& max) {
        return Vec2D{ UniformRand(min.x, max.x), UniformRand(min.y, max.y) };
    }
};