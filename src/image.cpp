#include "image.hpp"

#include <cmath>
#include <fstream>
#include <vector>

#include "light.hpp"
#include "shape.hpp"
#include "vectors.hpp"

Image::Image(int width, int height, float exposure, float gamma)
    : width(width), height(height), exposure(exposure), gamma(gamma) {}

void Image::setPixel(std::vector<int> &pixelValues, int row, int col,
                     const Color &color) {
    int redIndex{width * row * 3 + col * 3};
    pixelValues.at(redIndex) = static_cast<int>(std::round(color.r * 255));
    pixelValues.at(redIndex + 1) = static_cast<int>(std::round(color.g * 255));
    pixelValues.at(redIndex + 2) = static_cast<int>(std::round(color.b * 255));
}

std::vector<int> Image::rayTrace(const Camera &camera, Scene &scene,
                                 const LightRack &lightRack) {
    std::vector<int> pixelValues(width * height * 3, 0);

    for (int x{0}; x < width; ++x) {
        for (int y{0}; y < height; ++y) {
            const Vector2 screenCoord{
                2.0f * static_cast<float>(x) / static_cast<float>(width) - 1.0f,
                -2.0f * static_cast<float>(y) / static_cast<float>(height) +
                    1.0f};
            Ray ray{camera.makeRay(screenCoord)};
            if (scene.intersect(ray)) {
                Color intersectionColor{
                    lightRack.illuminate(ray, scene, camera)};
                setPixel(pixelValues, y, x,
                         intersectionColor.applyGamma(exposure, gamma).clamp());
            }
        }
    }

    return pixelValues;
}

void Image::saveImage(const std::vector<int> &pixelValues,
                      const std::string &filename) const {
    std::ofstream img{filename};
    img << "P3" << '\n';
    img << width << ' ' << height << '\n';
    img << 255 << '\n';

    for (int i : pixelValues) {
        img << i << ' ';
    }

    img.close();
}