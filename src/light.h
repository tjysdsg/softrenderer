#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "math/vector3.h"

struct Light {
    Light(const Vec3f &p, const float &i) : position(p), intensity(i) {}
    Vec3f position;
    float intensity;
};

#endif  // __LIGHT_H__
