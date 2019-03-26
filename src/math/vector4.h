#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include "math/math_defs.h"

struct Vector4 {
    union {
        struct {
            real_t x;
            real_t y;
            real_t z;
            real_t w;
        };
        real_t coord[4];
    };

    const real_t &operator[](int p_axis) const { return coord[p_axis]; }

    real_t &operator[](int p_axis) { return coord[p_axis]; }

    Vector4() { x = y = z = w = 0; }
    Vector4(real_t p_x, real_t p_y, real_t p_z, real_t p_w) {
        x = p_x;
        y = p_y;
        z = p_z;
        w = p_w;
    }
};

typedef Vector4 Vec4f;

#endif /* __VECTOR4_H__ */
