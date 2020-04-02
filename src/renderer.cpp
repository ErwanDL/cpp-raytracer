#include "renderer.hpp"

#include <cmath>
#include <fstream>
#include <vector>

#include "light.hpp"
#include "shape.hpp"
#include "vectors.hpp"

Renderer::Renderer(int width, int height, float exposure, float gamma)
    : width(width), height(height), exposure(exposure), gamma(gamma) {}

std::vector<int> Renderer::rayTrace(const Camera &camera, Scene &scene,
                                    const LightRack &lightRack) {
    std::vector<int> pixelValues(width * height * 3, 0);
    Point3 observerLocation{camera.getLocation()};

    for (int x{0}; x < width; ++x) {
        for (int y{0}; y < height; ++y) {
            const Vector2 screenCoord = screenCoordinateFromXY(x, y);
            Ray ray = camera.makeRay(screenCoord);
            const auto intersection = scene.intersect(ray);
            if (intersection) {
                Color intersectionColor = lightRack.illuminate(
                    intersection.value(), scene, observerLocation);
                setPixel(pixelValues, y, x,
                         intersectionColor.gammaCorrected(exposure, gamma));
            }
        }
    }

    return pixelValues;
}

Vector2 Renderer::screenCoordinateFromXY(int x, int y) const {
    return {2.0f * static_cast<float>(x) / static_cast<float>(width) - 1.0f,
            -2.0f * static_cast<float>(y) / static_cast<float>(height) + 1.0f};
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

void Renderer::setPixel(std::vector<int> &pixelValues, int row, int col,
                        const Color &color) {
    int redIndex{width * row * 3 + col * 3};
    pixelValues.at(redIndex) = convertTo8BitValue(color.r);
    pixelValues.at(redIndex + 1) = convertTo8BitValue(color.g);
    pixelValues.at(redIndex + 2) = convertTo8BitValue(color.b);
}

int Renderer::convertTo8BitValue(float f) {
    return static_cast<int>(std::round(f * 255));
}