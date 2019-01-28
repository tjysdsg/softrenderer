#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
#include "geometry.h"

void render2img(const char* name, const int width, const int height,
                std::vector<Vec3f> framebuffer);

#endif  //__IMAGE_H__
