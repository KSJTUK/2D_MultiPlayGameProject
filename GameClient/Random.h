#pragma once

class Random {
private:
    inline static std::random_device rd{ };
    inline static std::default_random_engine dre{ rd() };
    
    template <typename T> requires std::is_integral_v<T>
    using uniform_int = std::uniform_int_distribution<T>;
    
    template <typename T> requires std::is_floating_point_v<T>
    using uniform_real = std::uniform_real_distribution<T>;

    template <typename T> requires std::is_floating_point_v<T>
    using normal = std::normal_distribution<T>;
};