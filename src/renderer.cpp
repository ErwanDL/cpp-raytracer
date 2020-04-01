#include "renderer.hpp"

#include <cmath>
#include <fstream>
#include <vector>

#include "light.hpp"
#include "shape.hpp"
#include "vectors.hpp"

Renderer::Renderer(int width, int height, float exposure, float gamma)
    : width(width), height(height), exposure(exposure), gamma(gamma) {}

void Renderer::setPixel(std::vector<int> &pixelValues, int row, int col,
                        const Color &color) {
    int redIndex{width * row * 3 + col * 3};
    pixelValues.at(redIndex) = static_cast<int>(std::round(color.r * 255));
    pixelValues.at(redIndex + 1) = static_cast<int>(std::round(color.g * 255));
    pixelValues.at(redIndex + 2) = static_cast<int>(std::round(color.b * 255));
}

std::vector<int> Renderer::rayTrace(const Camera &camera, Scene &scene,
                                    const LightRack &lightRack) {
    std::vector<int> pixelValues(width * height * 3, 0);

    for (int x{0}; x < width; ++x) {
        for (int y{0}; y < height; ++y) {
            const Vector2 screenCoord{
                2.0f * static_cast<float>(x) / static_cast<float>(width) - 1.0f,
                -2.0f * static_cast<float>(y) / static_cast<float>(height) +
                    1.0f};
            Ray ray{camera.makeRay(screenCoord)};
            const auto intersection = scene.intersect(ray);
            if (intersection) {
                Color intersectionColor{
                    lightRack.illuminate(intersection.value(), scene, camera)};
                setPixel(pixelValues, y, x,
                         intersectionColor.applyGamma(exposure, gamma).clamp());
            }
        }
    }

    return pixelValues;
}

void Renderer::saveRenderer(const std::vector<int> &pixelValues,
                            const std::string &filename) const {
    std::ofstream renderer{filename};
    renderer << "P3" << '\n';
    renderer << width << ' ' << height << '\n';
    renderer << 255 << '\n';

    for (int i : pixelValues) {
        renderer << i << ' ';
    }

    renderer.close();
}