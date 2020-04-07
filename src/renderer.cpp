#include "renderer.hpp"

#include <cmath>
#include <exception>
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

std::vector<int> Renderer::rayTrace(const Camera &camera, int nReflexions,
                                    int supersamplingRate) const {
    std::vector<int> pixelValues(width * height * 3, 0);

    for (int x{0}; x < width; ++x) {
        for (int y{0}; y < height; ++y) {
            const auto offsets = getSupersamplingOffsets(supersamplingRate);
            Color averageColor{0.0f};
            for (const float subX : offsets) {
                for (const float subY : offsets) {
                    const Vector2 screenCoord =
                        screenCoordinateFromXY(static_cast<float>(x) + subX,
                                               static_cast<float>(y) + subY);
                    const Ray initialRay = camera.makeRay(screenCoord);
                    averageColor +=
                        shootRayRecursively(initialRay, nReflexions);
                }
            }
            averageColor /=
                static_cast<float>(supersamplingRate * supersamplingRate);
            setPixel(pixelValues, y, x,
                     averageColor.gammaCorrected(exposure, gamma));
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

    if (nReflexionsLeft == 0 || !intersection->material.isReflective()) {
        return intersectionColor;
    }
    const Ray reflectedRay{intersection->location,
                           ray.direction.reflected(intersection->normal)};
    const Color reflectedColor =
        intersection->material.specularColor *
        shootRayRecursively(reflectedRay, nReflexionsLeft - 1);
    return intersectionColor + reflectedColor;
}

std::vector<float> Renderer::getSupersamplingOffsets(int supersamplingRate) {
    if (supersamplingRate <= 0) {
        throw std::domain_error("Supersampling rate muste be >= 1");
    }
    const float unitSpacing = 1.0f / static_cast<float>(supersamplingRate);
    std::vector<float> offsets(supersamplingRate);
    for (int i = 0; i < supersamplingRate; ++i) {
        offsets[i] = unitSpacing + 2.0f * static_cast<float>(i) * unitSpacing;
    }
    return offsets;
}

Vector2 Renderer::screenCoordinateFromXY(float x, float y) const {
    return {2.0f * x / static_cast<float>(width) - 1.0f,
            -2.0f * y / static_cast<float>(height) + 1.0f};
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
                        const Color &color) const {
    int redIndex{width * row * 3 + col * 3};
    pixelValues.at(redIndex) = convertTo8BitValue(color.r);
    pixelValues.at(redIndex + 1) = convertTo8BitValue(color.g);
    pixelValues.at(redIndex + 2) = convertTo8BitValue(color.b);
}

int Renderer::convertTo8BitValue(float f) {
    return static_cast<int>(std::round(f * 255));
}