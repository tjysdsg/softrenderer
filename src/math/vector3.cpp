#include "vector3.h"

Vector3 operator*(real_t p_scalar, const Vector3 &p_vec) {
    return p_vec * p_scalar;
}

Vector3 vec3_cross(const Vector3 &p_a, const Vector3 &p_b) {
    return p_a.cross(p_b);
}

real_t vec3_dot(const Vector3 &p_a, const Vector3 &p_b) { return p_a.dot(p_b); }
