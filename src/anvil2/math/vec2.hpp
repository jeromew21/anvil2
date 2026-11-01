#pragma once

/**
 * @brief A 2D vector of 32-bit floats.
 */
struct Vec2 {
    float x = 0.0;
    float y = 0.0;

    static Vec2 add(const Vec2& a, const Vec2& b);
    static Vec2 negate(const Vec2& a);
    static Vec2 scale(const Vec2& a, float scalar);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(float scalar) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(float scalar);
};

