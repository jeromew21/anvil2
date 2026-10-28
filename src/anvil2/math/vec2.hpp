#pragma once

/**
 * @brief A 2D vector with basic arithmetic operations.
 * DEPRECATED: This struct may be removed in future versions.
 */
struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    static Vec2 add(const Vec2& a, const Vec2& b);
    static Vec2 negate(const Vec2& a);
    static Vec2 scale(const Vec2& a, double scalar);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(double scalar) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(double scalar);
};

