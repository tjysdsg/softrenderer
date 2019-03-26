#ifndef VECTOR3_H
#define VECTOR3_H

#include "math/math_defs.h"

struct Vector3 {
    union {
        struct {
            real_t x;
            real_t y;
            real_t z;
        };
        real_t coord[3];
    };

    const real_t &operator[](int p_axis) const { return coord[p_axis]; }

    real_t &operator[](int p_axis) { return coord[p_axis]; }

    real_t length() const;
    real_t length_squared() const;

    void normalize();
    Vector3 normalized() const;
    bool is_normalized() const;
    Vector3 inverse() const;

    void zero();

    Vector3 cross(const Vector3 &p_b) const {
        Vector3 ret((y * p_b.z) - (z * p_b.y), (z * p_b.x) - (x * p_b.z),
                    (x * p_b.y) - (y * p_b.x));

        return ret;
    }

    real_t dot(const Vector3 &p_b) const {
        return x * p_b.x + y * p_b.y + z * p_b.z;
    }

    Vector3 abs() const;
    Vector3 floor() const;
    Vector3 sign() const;
    Vector3 ceil() const;
    Vector3 round() const;

    real_t distance_to(const Vector3 &p_b) const;
    real_t distance_squared_to(const Vector3 &p_b) const;

    Vector3 project(const Vector3 &p_b) const;

    real_t angle_to(const Vector3 &p_b) const;

    Vector3 slide(const Vector3 &p_normal) const;
    Vector3 bounce(const Vector3 &p_normal) const;
    Vector3 reflect(const Vector3 &p_normal) const;

    /* Operators */
    Vector3 &operator+=(const Vector3 &p_v);
    Vector3 operator+(const Vector3 &p_v) const;
    Vector3 &operator-=(const Vector3 &p_v);
    Vector3 operator-(const Vector3 &p_v) const;
    Vector3 &operator*=(const Vector3 &p_v);
    Vector3 operator*(const Vector3 &p_v) const;
    Vector3 &operator/=(const Vector3 &p_v);
    Vector3 operator/(const Vector3 &p_v) const;

    Vector3 &operator*=(real_t p_scalar);
    Vector3 operator*(real_t p_scalar) const;
    Vector3 &operator/=(real_t p_scalar);
    Vector3 operator/(real_t p_scalar) const;

    Vector3 operator-() const;

    bool operator==(const Vector3 &p_v) const;
    bool operator!=(const Vector3 &p_v) const;
    bool operator<(const Vector3 &p_v) const;
    bool operator<=(const Vector3 &p_v) const;

    Vector3() { x = y = z = 0; }
    Vector3(real_t p_x, real_t p_y, real_t p_z) {
        x = p_x;
        y = p_y;
        z = p_z;
    }
};

typedef Vector3 Vec3f;

#endif  // VECTOR3_H
