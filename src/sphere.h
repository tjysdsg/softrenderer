#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "geometry.h"
#include "macro.h"
#include "material.h"

#define GET_MESH_TYPE(m) (*((mesh_type *)((void *)(m))))

typedef enum { BOX, SPHERE, PLANE } mesh_type;

struct Mesh {
    mesh_type type;
    Vec3f center;
    Material material;
};

struct Sphere {
    mesh_type type;
    Vec3f center;
    Material material;

    float radius;

    __RAYTRACER_API__ Sphere() : type(SPHERE), center(), radius(), material() {}
    __RAYTRACER_API__ Sphere(const Vec3f &c, const float &r, const Material &m)
        : type(SPHERE), center(c), radius(r), material(m) {}

    __RAYTRACER_API__ bool ray_intersect(__in__ const Vec3f orig,
                                         __in__ const Vec3f dir,
                                         __in_out__ float &t0) const {
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
