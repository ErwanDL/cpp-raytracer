#include "renderer.hpp"

#include <cmath>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "light.hpp"
#include "random.hpp"
#include "shape.hpp"
#include "utils.hpp"
#include "vectors.hpp"

Renderer::Renderer(const Intersectable &scene, const Light &lights, int width,
                   int height, const ReflectionGenerator &reflectionGenerator,
                   const SupersamplingStrategy &superSampler, float exposure,
                   float gamma)
    : scene(scene),
      lights(lights),
      width(width),
      height(height),
      reflectionGenerator(reflectionGenerator),
      superSampler(superSampler),
      exposure(exposure),
      gamma(gamma) {}

std::vector<int> Renderer::rayTrace(const Camera &camera, int nBounces) const {
    std::vector<int> pixelValues(width * height * 3, 0);
    const auto start = std::chrono::steady_clock::now();
    std::cout << "Starting render...\n";

    for (int x{0}; x < width; ++x) {
        displayProgress(static_cast<float>(x) / static_cast<float>(width));
        for (int y{0}; y < height; ++y) {
            const auto offsets = superSampler.getSupersamplingOffsets();
            Color averageColor{0.0f};
            for (const auto offset : offsets) {
                const Vector2 screenCoord = screenCoordinateFromXY(
                    static_cast<float>(x) + offset.first,
                    static_cast<float>(y) + offset.second);
                const Ray initialRay = camera.makeRay(screenCoord);
                averageColor += shootRayRecursively(initialRay, nBounces);
            }
            averageColor /= static_cast<float>(offsets.size());
            setPixel(pixelValues, y, x,
                     averageColor.gammaCorrected(exposure, gamma));
        }
    }

    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
    std::cout << "\nScene rendered in "
              << static_cast<float>(duration.count()) / 1000.0f
              << " seconds.\n";

    return pixelValues;
}

void Renderer::displayProgress(float progressRatio) {
    constexpr int nChars = 50;
    const int nSquares = static_cast<int>(std::round(progressRatio * nChars));
    std::string progressBar;
    for (int i = 0; i < nSquares; ++i) {
        progressBar.append("■");
    }
    for (int i = nSquares; i < nChars; ++i) {
        progressBar.append("-");
    }
    std::cout << '\r' << progressBar << ' '
              << static_cast<int>(std::round(100 * progressRatio)) << '%'
              << std::flush;
}

Color Renderer::shootRayRecursively(const Ray &ray, int nBouncesLeft) const {
    const auto intersection = scene.intersect(ray);
    if (!intersection) {
        return skyColor;
    }
    const Color intersectionColor =
        lights.illuminate(intersection.value(), scene, ray.origin);

    if (nBouncesLeft == 0 || intersection->material.specularity() < 0.01f) {
        return intersectionColor;
    }

    const Vector3 perfectReflectionDirection =
        ray.direction.reflected(intersection->normal);

    auto reflections = reflectionGenerator.getRandomReflections(
        perfectReflectionDirection, intersection->material);

    Color reflectedColor{0.0f};
    int shotRays = 0;
    for (const Vector3 reflection : reflections) {
        if (reflection.dot(intersection->normal) > 0.0f) {
            const Ray reflectedRay{intersection->location, reflection};
            reflectedColor +=
                shootRayRecursively(reflectedRay, nBouncesLeft - 1);
            shotRays += 1;
        }
    }

    return intersectionColor +
           intersection->material.specularColor *
               (reflectedColor / static_cast<float>(shotRays));
}

Vector2 Renderer::screenCoordinateFromXY(float x, float y) const {
    return {2.0f * x / static_cast<float>(width) - 1.0f,
            -2.0f * y / static_cast<float>(height) + 1.0f};
}

void Renderer::saveRender(const std::vector<int> &pixelValues,
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
