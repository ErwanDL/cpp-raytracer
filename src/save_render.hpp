#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "material.hpp"
#include <fstream>
#include <string>
#include <vector>

int to8Bit(float f) { return static_cast<int>(std::round(f * 255)); }

void saveRenderToPPM(const std::vector<std::vector<Color>>& render, const std::string& filename) {
    std::ofstream outfile{filename};
    int width = render[0].size();
    int height = render.size();

    outfile << "P3" << '\n';
    outfile << width << ' ' << height << '\n';
    outfile << 255 << '\n';

    for (const auto& line : render) {
        for (const Color& pixel : line) {
            outfile << to8Bit(pixel.r) << ' ' << to8Bit(pixel.g) << ' ' << to8Bit(pixel.b) << ' ';
        }
    }

    outfile.close();
}
#endif