#include "vector3.h"
#include <cmath>

Vector3 Vector3::abs() const {
    return Vector3(std::abs(x), std::abs(y), std::abs(z));
}

Vector3 Vector3::sign() const { return Vector3(SGN(x), SGN(y), SGN(z)); }

Vector3 Vector3::floor() const {
    return Vector3(std::floor(x), std::floor(y), std::floor(z));
}

Vector3 Vector3::ceil() const {
    return Vector3(std::ceil(x), std::ceil(y), std::ceil(z));
}

Vector3 Vector3::round() const {
    return Vector3(std::round(x), std::round(y), std::round(z));
}

real_t Vector3::distance_to(const Vector3 &p_b) const {
    return (p_b - *this).length();
}

real_t Vector3::distance_squared_to(const Vector3 &p_b) const {
    return (p_b - *this).length_squared();
}

Vector3 Vector3::project(const Vector3 &p_b) const {
    return p_b * (dot(p_b) / p_b.length_squared());
}

real_t Vector3::angle_to(const Vector3 &p_b) const {
    return std::atan2(cross(p_b).length(), dot(p_b));
}

/* Operators */
Vector3 &Vector3::operator+=(const Vector3 &p_v) {
    x += p_v.x;
    y += p_v.y;
    z += p_v.z;
    return *this;
}

Vector3 Vector3::operator+(const Vector3 &p_v) const {
    return Vector3(x + p_v.x, y + p_v.y, z + p_v.z);
}

Vector3 &Vector3::operator-=(const Vector3 &p_v) {
    x -= p_v.x;
    y -= p_v.y;
    z -= p_v.z;
    return *this;
}

Vector3 Vector3::operator-(const Vector3 &p_v) const {
    return Vector3(x - p_v.x, y - p_v.y, z - p_v.z);
}

Vector3 &Vector3::operator*=(const Vector3 &p_v) {
    x *= p_v.x;
    y *= p_v.y;
    z *= p_v.z;
    return *this;
}
Vector3 Vector3::operator*(const Vector3 &p_v) const {
    return Vector3(x * p_v.x, y * p_v.y, z * p_v.z);
}

Vector3 &Vector3::operator/=(const Vector3 &p_v) {
    x /= p_v.x;
    y /= p_v.y;
    z /= p_v.z;
    return *this;
}

Vector3 Vector3::operator/(const Vector3 &p_v) const {
    return Vector3(x / p_v.x, y / p_v.y, z / p_v.z);
}

Vector3 &Vector3::operator*=(real_t p_scalar) {
    x *= p_scalar;
    y *= p_scalar;
    z *= p_scalar;
    return *this;
}

Vector3 operator*(real_t p_scalar, const Vector3 &p_vec) {
    return p_vec * p_scalar;
}

Vector3 Vector3::operator*(real_t p_scalar) const {
    return Vector3(x * p_scalar, y * p_scalar, z * p_scalar);
}

Vector3 &Vector3::operator/=(real_t p_scalar) {
    x /= p_scalar;
    y /= p_scalar;
    z /= p_scalar;
    return *this;
}

Vector3 Vector3::operator/(real_t p_scalar) const {
    return Vector3(x / p_scalar, y / p_scalar, z / p_scalar);
}

Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

bool Vector3::operator==(const Vector3 &p_v) const {
    return (x == p_v.x && y == p_v.y && z == p_v.z);
}

bool Vector3::operator!=(const Vector3 &p_v) const {
    return (x != p_v.x || y != p_v.y || z != p_v.z);
}

bool Vector3::operator<(const Vector3 &p_v) const {
    if (x == p_v.x) {
        if (y == p_v.y)
            return z < p_v.z;
        else
            return y < p_v.y;
    } else {
        return x < p_v.x;
    }
}

bool Vector3::operator<=(const Vector3 &p_v) const {
    if (x == p_v.x) {
        if (y == p_v.y)
            return z <= p_v.z;
        else
            return y < p_v.y;
    } else {
        return x < p_v.x;
    }
}

Vector3 vec3_cross(const Vector3 &p_a, const Vector3 &p_b) {
    return p_a.cross(p_b);
}

real_t vec3_dot(const Vector3 &p_a, const Vector3 &p_b) { return p_a.dot(p_b); }

real_t Vector3::length() const {
    real_t x2 = x * x;
    real_t y2 = y * y;
    real_t z2 = z * z;

    return std::sqrt(x2 + y2 + z2);
}

real_t Vector3::length_squared() const {
    real_t x2 = x * x;
    real_t y2 = y * y;
    real_t z2 = z * z;

    return x2 + y2 + z2;
}

void Vector3::normalize() {
    real_t l = length();
    if (l == 0) {
        x = y = z = 0;
    } else {
        x /= l;
        y /= l;
        z /= l;
    }
}

Vector3 Vector3::normalized() const {
    Vector3 v = *this;
    v.normalize();
    return v;
}

bool Vector3::is_normalized() const {
    // use length_squared() instead of length() to avoid sqrt(), makes it more
    // stringent.
    // FIXME approximately equals
    return length_squared() == 1.0;
}

Vector3 Vector3::inverse() const { return Vector3(1.0 / x, 1.0 / y, 1.0 / z); }

void Vector3::zero() { x = y = z = 0; }

// slide returns the component of the vector along the given plane, specified by
// its normal vector.
Vector3 Vector3::slide(const Vector3 &p_normal) const {
    return *this - p_normal * this->dot(p_normal);
}

Vector3 Vector3::bounce(const Vector3 &p_normal) const {
    return -reflect(p_normal);
}

Vector3 Vector3::reflect(const Vector3 &p_normal) const {
    return 2.0 * p_normal * this->dot(p_normal) - *this;
}
