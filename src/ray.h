#ifndef __RAY_H__
#define __RAY_H__
#include "math/vector3.h"

struct Ray {
    Vec3f origin;
    Vec3f direction;
    float len;  // when initializing, this variable contains information about
                // max distance the ray can go, and is less than or equal to 0
                // if infinite max distance
    // if ray_intersect returned true, this variable contains the distance
    // between contact point and the ray's origin
    bool hit = false;
    Vec3f contact = Vec3f();

    Ray() : origin(), direction(), len(0) {}
    Ray(Vec3f orig, Vec3f dir) : origin(orig), direction(dir), len(0) {}
    Ray(Vec3f orig, Vec3f dir, float max_length)
        : origin(orig), direction(dir), len(max_length) {}
};

#endif  // __RAY_H__
