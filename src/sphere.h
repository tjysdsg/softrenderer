#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "geometry.h"
#include "macro.h"
#include "material.h"

class Mesh {
public:
    virtual bool ray_intersect(__in__ const Vec3f orig, __in__ const Vec3f dir,
                               __in_out__ float &t0) const = 0;
    virtual Material get_material() const = 0;
    virtual ~Mesh(){};
    Mesh(){};
};

class Sphere : public Mesh {
private:
    Vec3f center;
    float radius;
    Material material;

public:
    __RAYTRACER_API__ Sphere() : center(), radius(), material() {}
    __RAYTRACER_API__ Sphere(const Vec3f &c, const float &r, const Material &m)
        : center(c), radius(r), material(m) {}

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
    __RAYTRACER_API__ Material get_material() const { return material; }
    __RAYTRACER_API__ float get_radius() const { return radius; }
    __RAYTRACER_API__ Vec3f get_center() const { return center; }
};
#endif  // __SPHERE_H__
