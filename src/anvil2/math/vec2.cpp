#include "vec2.hpp"

Vec2f Vec2f::add(const Vec2f& a, const Vec2f& b) {
    return {a.x + b.x, a.y + b.y};
}

Vec2f Vec2f::negate(const Vec2f& a) {
    return {-a.x, -a.y};
}

Vec2f Vec2f::scale(const Vec2f& a, const float scalar) {
    return {a.x * scalar, a.y * scalar};
}

Vec2f Vec2f::operator+(const Vec2f& other) const {
    return add(*this, other);
}

Vec2f Vec2f::operator-(const Vec2f& other) const {
    return add(*this, negate(other));
}

Vec2f Vec2f::operator*(const float scalar) const {
    return scale(*this, scalar);
}

Vec2f& Vec2f::operator+=(const Vec2f& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Vec2f& Vec2f::operator-=(const Vec2f& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Vec2f& Vec2f::operator*=(const float scalar) {
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}
