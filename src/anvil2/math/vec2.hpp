#pragma once

/**
 * @brief A 2D vector of 32-bit floats.
 */
struct Vec2f {
    float x = 0.0;
    float y = 0.0;

    static Vec2f add(const Vec2f& a, const Vec2f& b);
    static Vec2f negate(const Vec2f& a);
    static Vec2f scale(const Vec2f& a, float scalar);

    Vec2f operator+(const Vec2f& other) const;
    Vec2f operator-(const Vec2f& other) const;
    Vec2f operator*(float scalar) const;

    Vec2f& operator+=(const Vec2f& other);
    Vec2f& operator-=(const Vec2f& other);
    Vec2f& operator*=(float scalar);
};

