#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "math/vector3.h"
#include "math/vector4.h"

struct Material {
    Vec3f diffuse_color;
    Material(const float r, const Vec4f a, const Vec3f color, const float spec)
        : refractive_index(r),
          albedo(a),
          diffuse_color(color),
          specular_exponent(spec) {}
    Material()
        : refractive_index(1),
          albedo(1, 0, 0, 0),
          diffuse_color(),
          specular_exponent() {}
    float specular_exponent;
    float refractive_index;
    Vec4f albedo;
};

#endif  // __MATERIAL_H__
