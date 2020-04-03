#include "renderer.hpp"

#include <cmath>
#include <fstream>
#include <vector>

#include "light.hpp"
#include "shape.hpp"
#include "utils.hpp"
#include "vectors.hpp"

Renderer::Renderer(const Intersectable &scene, const Light &lights, int width,
                   int height, float exposure, float gamma)
    : scene(scene),
      lights(lights),
      width(width),
      height(height),
      exposure(exposure),
      gamma(gamma) {}

std::vector<int> Renderer::rayTrace(const Camera &camera, int nReflexions) {
    std::vector<int> pixelValues(width * height * 3, 0);

    for (int x{0}; x < width; ++x) {
        for (int y{0}; y < height; ++y) {
            const Vector2 screenCoord = screenCoordinateFromXY(x, y);
            const Ray initialRay = camera.makeRay(screenCoord);
            const Color pixelColor =
                shootRayRecursively(initialRay, nReflexions);
            setPixel(pixelValues, y, x,
                     pixelColor.gammaCorrected(exposure, gamma));
        }
    }

    return pixelValues;
}

Color Renderer::shootRayRecursively(const Ray &ray, int nReflexionsLeft) const {
    const auto intersection = scene.intersect(ray);
    if (!intersection) {
        return skyColor;
    }
    const Color intersectionColor =
        lights.illuminate(intersection.value(), scene, ray.origin);

    if (nReflexionsLeft == 0 ||
        Math::floatingPointEquality(intersection->material.specular, 0.0f)) {
        return intersectionColor;
    }
    const Ray reflectedRay{intersection->location,
                           ray.direction.reflected(intersection->normal)};
    const Color reflectedColor =
        intersection->material.specular *
        shootRayRecursively(reflectedRay, nReflexionsLeft - 1);
    return intersectionColor + reflectedColor;
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