#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "geometry.h"
struct Material {
    Vec3f diffuse_color;
    Material(const Vec3f &color) : diffuse_color(color) {}
    Material() : diffuse_color() {}
};

#endif  // __MATERIAL_H__
