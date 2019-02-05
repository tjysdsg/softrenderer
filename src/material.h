#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "geometry.h"
struct Material {
    Vec3f diffuse_color;
    Material(const Vec3f a, const Vec3f color, const float spec)
        : albedo(a), diffuse_color(color), specular_exponent(spec) {}
    Material() : albedo(1, 0, 0), diffuse_color(), specular_exponent() {}
    float specular_exponent;
    Vec3f albedo;
};

#endif  // __MATERIAL_H__
