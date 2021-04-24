#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "material.hpp"
#include <fstream>
#include <lodepng.h>
#include <string>
#include <vector>

unsigned char to8Bit(float f) { return static_cast<unsigned char>(std::round(f * 255)); }

void saveRenderToPNG(const std::vector<std::vector<Color>>& render, const std::string& filename) {
    int width = render[0].size();
    int height = render.size();

    std::vector<unsigned char> img(width * height * 3);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int idx = 3 * (width * y + x);
            img[idx] = to8Bit(render[y][x].r);
            img[idx + 1] = to8Bit(render[y][x].g);
            img[idx + 2] = to8Bit(render[y][x].b);
        }
    }

    lodepng::encode(filename, img, width, height, LCT_RGB);
}
#endif