#include "geometry.h"
#include "image.h"
#include "sphere.h"

Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir, const Sphere &sphere) {
    float sphere_dist = std::numeric_limits<float>::max();
    if (!sphere.ray_intersect(orig, dir, sphere_dist)) {
        return Vec3f(0.2, 0.7, 0.8);  // background color
    }
    return Vec3f(0.4, 0.4, 0.3);
}

int main() {
    const int width = 1024;
    const int height = 768;
    std::vector<Vec3f> framebuffer(width * height);
    float fov = 3.1415926 / 4;
    Sphere sphere(Vec3f(0, 1.0f, -50.0f), 5.0f);

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) *
                      width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();
            framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), dir, sphere);
        }
    }

    render2img("out.ppm", width, height, framebuffer);
    return 0;
}
