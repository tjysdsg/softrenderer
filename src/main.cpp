#include "geometry.h"
#include "image.h"
#include "light.h"
#include "macro.h"
#include "sphere.h"

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
    float diffuse_light_intensity = 0;
    for (size_t i = 0; i < lights.size(); i++) {
        Vec3f light_dir = (lights[i].position - point).normalize();
        diffuse_light_intensity +=
            lights[i].intensity * std::max(0.f, light_dir * N);
    }
    return material.diffuse_color * diffuse_light_intensity;
}

int main() {
    const int width = 1024;
    const int height = 768;
    std::vector<Vec3f> framebuffer(width * height);
    float fov = 3.1415926 / 4;
    //
    Material ivory(Vec3f(0.4, 0.4, 0.3));
    Material red_rubber(Vec3f(0.3, 0.1, 0.1));
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, ivory));
    spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2, red_rubber));
    spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, red_rubber));
    spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, ivory));
    //
    std::vector<Light> lights;
    lights.push_back(Light(Vec3f(-20, 20, 20), 1.5));

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) *
                      width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();
            framebuffer[i + j * width] =
                raycast_test(Vec3f(0, 0, 0), dir, spheres, lights);
        }
    }

    render2img("out.ppm", width, height, framebuffer);
    return 0;
}
