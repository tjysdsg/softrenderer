#include "config.h"
#include "geometry.h"
#include "image.h"
#include "light.h"
#include "macro.h"
#include "material.h"
#include "sphere.h"

Vec3f reflect(const Vec3f I, const Vec3f N) { return I - N * 2.f * (I * N); }

Vec3f refract(const Vec3f I, const Vec3f N,
              const float refractive_index) {  // Snell's law
    float cosi = -std::max(-1.f, std::min(1.f, I * N));
    float etai = 1, etat = refractive_index;
    Vec3f n = N;
    if (cosi < 0) {  // if the ray is inside the object, swap the indices and
                     // invert the normal to get the correct result
        cosi = -cosi;
        std::swap(etai, etat);
        n = -N;
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? Vec3f(0, 0, 0) : I * eta + n * (eta * cosi - sqrtf(k));
}

// TODO: refactor this rubbish API
__RAYTRACER_API__ bool scene_intersect(__in__ const Vec3f orig,
                                       __in__ const Vec3f dir,
                                       __in__ const std::vector<Sphere> spheres,
                                       __out__ Vec3f &hit, __out__ Vec3f &N,
                                       __out__ Material &material) {
    float spheres_dist = std::numeric_limits<float>::max();
    for (size_t i = 0; i < spheres.size(); i++) {
        float dist_i;
        if (spheres[i].ray_intersect(orig, dir, dist_i) &&
            dist_i < spheres_dist) {
            spheres_dist = dist_i;
            hit = orig + dir * dist_i;
            N = (hit - spheres[i].get_center()).normalize();
            material = spheres[i].get_material();
        }
    }
    return spheres_dist < 1000;
}

Vec3f emit_ray(const Vec3f &orig, const Vec3f &dir,
               const std::vector<Sphere> spheres, std::vector<Light> lights,
               size_t depth = 0) {
    Vec3f point, N;
    Material material;
    // if there is no reflection and intersection with other object, return the
    // background color
    if (depth > reflection_depth ||
        !scene_intersect(orig, dir, spheres, point, N, material)) {
        return Vec3f(0.2, 0.7, 0.8);  // background color
    }
    // calculate reflection
    Vec3f reflect_dir = reflect(dir, N).normalize();
    Vec3f reflect_orig =
        reflect_dir * N < 0
            ? point - N * 1e-3
            : point + N * 1e-3;  // offset the original point to avoid occlusion
                                 // by the object itself

    // emit ray recursively until depth is greater than
    Vec3f reflect_color =
        emit_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);

    // calculate refraction
    Vec3f refract_dir = refract(dir, N, material.refractive_index).normalize();
    Vec3f refract_orig =
        refract_dir * N < 0 ? point - N * 1e-3 : point + N * 1e-3;
    Vec3f refract_color =
        emit_ray(refract_orig, refract_dir, spheres, lights, depth + 1);

    // calculate light
    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (size_t i = 0; i < lights.size(); i++) {
        Vec3f light_dir = (lights[i].position - point).normalize();
        float light_distance = (lights[i].position - point).norm();

        // shadow
        Vec3f shadow_orig =
            light_dir * N < 0
                ? point - N * 1e-3
                : point + N * 1e-3;  // checking if the point lies in the shadow
                                     // of the lights[i]
        Vec3f shadow_pt, shadow_N;
        Material tmpmaterial;
        if (scene_intersect(shadow_orig, light_dir, spheres, shadow_pt,
                            shadow_N, tmpmaterial) &&
            (shadow_pt - shadow_orig).norm() < light_distance)
            continue;

        // calculate light intensity
        diffuse_light_intensity +=
            lights[i].intensity * std::max(0.f, light_dir * N);
        specular_light_intensity +=
            powf(std::max(0.f, -reflect(-light_dir, N) * dir),
                 material.specular_exponent) *
            lights[i].intensity;
    }
    return material.diffuse_color * diffuse_light_intensity *
               material.albedo[0] +
           Vec3f(1., 1., 1.) * specular_light_intensity * material.albedo[1] +
           reflect_color * material.albedo[2] +
           refract_color * material.albedo[3];
}

int main() {
    const int width = 1024;
    const int height = 768;
    std::vector<Vec3f> framebuffer(width * height);
    float fov = 3.1415926 / 4;
    // add some materials
    Material ivory(1.0, Vec4f(0.6, 0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3), 50.);
    Material glass(1.5, Vec4f(0.0, 0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8), 125.);
    Material red_rubber(1.0, Vec4f(0.9, 0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1),
                        10.);
    Material mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0),
                    1425.);

    // add some spheres with material into the scene
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, ivory));
    spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2, glass));
    spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, red_rubber));
    spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, mirror));
    // add some light sources
    std::vector<Light> lights;
    lights.push_back(Light(Vec3f(-20, 20, 20), 1.5));
    lights.push_back(Light(Vec3f(30, 50, -25), 1.8));
    lights.push_back(Light(Vec3f(30, 20, 30), 1.7));

    // render procedures
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) *
                      width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();
            Vec3f c = emit_ray(Vec3f(0, 0, 0), dir, spheres, lights);
            float max = std::max(c[0], std::max(c[1], c[2]));
            if (max > 1) c = c * (1. / max);
            framebuffer[i + j * width] = c;
        }
    }
    // save the result to file
    render2img("out.ppm", width, height, framebuffer);
    return 0;
}
