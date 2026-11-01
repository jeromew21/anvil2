#include "vec2.hpp"

Vec2 Vec2::add(const Vec2& a, const Vec2& b) {
    return {a.x + b.x, a.y + b.y};
}

Vec2 Vec2::negate(const Vec2& a) {
    return {-a.x, -a.y};
}

Vec2 Vec2::scale(const Vec2& a, const float scalar) {
    return {a.x * scalar, a.y * scalar};
}

Vec2 Vec2::operator+(const Vec2& other) const {
    return add(*this, other);
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return add(*this, negate(other));
}

Vec2 Vec2::operator*(const float scalar) const {
    return scale(*this, scalar);
}

Vec2& Vec2::operator+=(const Vec2& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Vec2& Vec2::operator*=(const float scalar) {
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}
