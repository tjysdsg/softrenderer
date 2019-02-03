#include "geometry.h"
#include "image.h"
#include "light.h"
#include "macro.h"
#include "material.h"
#include "sphere.h"
Vec3f reflect(const Vec3f &I, const Vec3f &N) { return I - N * 2.f * (I * N); }

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
            N = (hit - spheres[i].center).normalize();
            material = spheres[i].material;
        }
    }
    return spheres_dist < 1000;
}

Vec3f raycast_test(const Vec3f &orig, const Vec3f &dir,
                   const std::vector<Sphere> spheres,
                   std::vector<Light> lights) {
    Vec3f point, N;
    Material material;
    if (!scene_intersect(orig, dir, spheres, point, N, material)) {
        return Vec3f(0.2, 0.7, 0.8);  // background color
    }
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
           Vec3f(1., 1., 1.) * specular_light_intensity * material.albedo[1];
}

int main() {
    const int width = 1024;
    const int height = 768;
    std::vector<Vec3f> framebuffer(width * height);
    float fov = 3.1415926 / 4;
    //
    Material ivory(Vec2f(0.6, 0.3), Vec3f(0.4, 0.4, 0.3), 50.);
    Material red_rubber(Vec2f(0.9, 0.1), Vec3f(0.3, 0.1, 0.1), 10.);
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, ivory));
    spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2, red_rubber));
    spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, red_rubber));
    spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, ivory));
    //
    std::vector<Light> lights;
    lights.push_back(Light(Vec3f(-20, 20, 20), 1.5));
    lights.push_back(Light(Vec3f(30, 50, -25), 1.8));
    lights.push_back(Light(Vec3f(30, 20, 30), 1.7));

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) *
                      width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();
            Vec3f c = raycast_test(Vec3f(0, 0, 0), dir, spheres, lights);
            float max = std::max(c[0], std::max(c[1], c[2]));
            if (max > 1) c = c * (1. / max);
            framebuffer[i + j * width] = c;
        }
    }
    render2img("out.ppm", width, height, framebuffer);
    return 0;
}
