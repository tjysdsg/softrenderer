#include "geometry.h"
#include "image.h"

int main() {
    const int width = 1024;
    const int height = 768;
    std::vector<Vec3f> framebuffer(width * height);

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            framebuffer[i + j * width] =
                Vec3f(j / float(height), i / float(width), 0);
        }
    }

    render2img(width, height, framebuffer);
    return 0;
}
