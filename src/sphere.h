#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "macro.h"
#include "material.h"
#include "vec.h"

class Mesh {
public:
    Vec3f center;
    Material material;
    Mesh() : center(), material() {}
    Mesh(const Vec3f _center, const Material _material)
        : center(_center), material(_material) {}
    virtual ~Mesh() {}
    virtual bool ray_intersect(const Vec3f orig, const Vec3f dir,
                               float &t0) const {
        return false;
    }
};

class Sphere : public Mesh {
public:
    float radius;
    Sphere() : Mesh(), radius() {}
    Sphere(const Vec3f c, const float r, const Material m)
        : Mesh(c, m), radius(r) {}
    ~Sphere() {}

    bool ray_intersect(const Vec3f orig, const Vec3f dir, float &t0) const {
        Vec3f L = center - orig;
        float tca = L * dir;
        float d2 = L * L - tca * tca;
        if (d2 > radius * radius) return false;
        float thc = sqrtf(radius * radius - d2);
        t0 = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return false;
        return true;
    }
};

#endif  // __SPHERE_H__
