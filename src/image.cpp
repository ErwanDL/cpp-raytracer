#include "image.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include "shape.hpp"
#include "vectors.hpp"

Color::Color() {}
Color::Color(float f) {
    assert(
        f <= 1.0f &&
        "Floats given to the Color constructor must be between 0.0f and 1.0f");
    r = f;
    g = f;
    b = f;
}
Color::Color(float red, float green, float blue) {
    assert(
        red <= 1.0f && blue <= 1.0f && green <= 1.0f &&
        "Floats given to the Color constructor must be between 0.0f and 1.0f");
    r = red;
    g = green;
    b = blue;
}

Color &Color::clamp(float min, float max) {
    r = std::clamp(r, min, max);
    g = std::clamp(g, min, max);
    b = std::clamp(b, min, max);
    return *this;
}

Color &Color::applyGamma(float exposure, float gamma) {
    r = std::pow(r * exposure, gamma);
    g = std::pow(g * exposure, gamma);
    b = std::pow(b * exposure, gamma);
    return *this;
}

Image::Image(int width, int height, float exposure, float gamma)
    : width(width),
      height(height),
      data(width * height * 3, 0),
      exposure(exposure),
      gamma(gamma) {}

void Image::setPixel(int row, int col, const Color &color) {
    int redIndex{width * row * 3 + col * 3};
    data.at(redIndex) = static_cast<int>(std::round(color.r * 255));
    data.at(redIndex + 1) = static_cast<int>(std::round(color.g * 255));
    data.at(redIndex + 2) = static_cast<int>(std::round(color.b * 255));
}

void Image::rayTrace(const Camera &camera, Shape &scene) {
    for (int x{0}; x < width; ++x) {
        for (int y{0}; y < height; ++y) {
            const Vector2 screenCoord{
                2.0f * static_cast<float>(x) / static_cast<float>(width) - 1.0f,
                -2.0f * static_cast<float>(y) / static_cast<float>(height) +
                    1.0f};
            Ray ray{camera.makeRay(screenCoord)};
            Intersection intersection{ray};
            if (scene.intersect(intersection)) {
                setPixel(y, x,
                         intersection.getColor()
                             .applyGamma(exposure, gamma)
                             .clamp());
            }
        }
    }
}

void Image::saveImage(const std::string &filename) const {
    std::ofstream img{filename};
    img << "P3" << '\n';
    img << width << ' ' << height << '\n';
    img << 255 << '\n';

    for (int i : data) {
        img << i << ' ';
    }

    img.close();
}