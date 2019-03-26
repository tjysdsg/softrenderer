#ifndef __MESH_H__
#define __MESH_H__
#include "macro.h"
#include "material.h"
#include "math/vector3.h"
#include "ray.h"

#define SMALL_POSITIVE_DOUBLE (1e-5)

class Mesh {
public:
    Vec3f center;
    Material material;
    Mesh() : center(), material() {}
    Mesh(const Vec3f _center, const Material _material)
        : center(_center), material(_material) {}
    virtual ~Mesh() {}
    virtual bool ray_intersect(Ray &r) const { return false; }
};

class Sphere : public Mesh {
public:
    float radius;
    Sphere() : Mesh(), radius() {}
    Sphere(const Vec3f c, const float r, const Material m)
        : Mesh(c, m), radius(r) {}
    ~Sphere() {}

    bool ray_intersect(Ray &r) const {
        float t0 = r.len;
        Vec3f orig = r.origin;
        Vec3f dir = r.direction;

        Vec3f L = center - orig;
        float tca = L.dot(dir);
        float d2 = L.dot(L) - tca * tca;
        float r_square = radius * radius;
        if (d2 > r_square) {
            return false;
        }
        float thc = sqrtf(r_square - d2);
        t0 = tca - thc;
        float t1 = tca + thc;
        if (t0 < 0) {
            t0 = t1;
        }
        if (t0 < 0) {
            return false;
        }
        r.len = t0;
        return true;
    }
};
#endif  // __MESH_H__
