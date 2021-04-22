#include "renderer.hpp"

#include <chrono>
#include <cmath>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "light.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "utils.hpp"
#include "vectors.hpp"

Renderer::Renderer(const Intersectable& scene, const Light& lights, int width, int height,
                   float exposure, float gamma)
    : scene(scene), lights(lights), width(width), height(height), exposure(exposure), gamma(gamma) {
}

std::vector<int> Renderer::rayTrace(const Camera& camera, int nBounces,
                                    int maxReflectionSamples) const {
    std::vector<int> pixelValues(width * height * 3, 0);
    const auto start = std::chrono::steady_clock::now();
    std::cout << "Starting render...\n";

    for (int x{0}; x < width; ++x) {
        displayProgress(static_cast<float>(x) / static_cast<float>(width));
        for (int y = 0; y < height; ++y) {
            const Vector2 screenCoord =
                screenCoordinateFromXY(static_cast<float>(x), static_cast<float>(y));
            const Ray initialRay = camera.makeRay(screenCoord);
            auto color = shootRayRecursively(initialRay, nBounces, maxReflectionSamples);

            setPixel(pixelValues, y, x, color.gammaCorrected(exposure, gamma));
        }
    }

    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
    std::cout << "\nScene rendered in " << static_cast<float>(duration.count()) / 1000.0f
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
    std::cout << '\r' << progressBar << ' ' << static_cast<int>(std::round(100 * progressRatio))
              << '%' << std::flush;
}

Color Renderer::shootRayRecursively(const Ray& ray, int nBouncesLeft,
                                    int maxReflectionSamples) const {
    const auto intersection = scene.intersect(ray);
    if (!intersection) {
        return skyColor;
    }
    const Color intersectionColor = lights.illuminate(intersection.value(), scene, ray.origin);
    const Material& material = intersection->material;

    if (nBouncesLeft == 0 || material.specularity < 1.0f) {
        return intersectionColor;
    }

    const Vector3 perfectReflectionDirection = ray.direction.reflected(intersection->normal);

    Color reflectedColor{0.0f};
    int nSamples = 1 + maxReflectionSamples;
    for (int i = 0; i < nSamples; ++i) {
        Vector3 sampledDirection =
            Math::sampleHemisphere(perfectReflectionDirection, material.smoothness);
        // Reflected rays that would shoot beneath the surface are reflected about the
        // perfect reflection direction, back above the surface
        if (sampledDirection.dot(intersection->normal) < 0.0f) {
            sampledDirection = (-sampledDirection).reflected(perfectReflectionDirection);
        }
        const Ray reflectedRay{intersection->location, sampledDirection};
        reflectedColor += shootRayRecursively(reflectedRay, nBouncesLeft - 1, maxReflectionSamples);
    }
    reflectedColor /= nSamples;

    return intersectionColor +
           reflectedColor * (material.metal ? material.albedo : material.specularity);
}

Vector2 Renderer::screenCoordinateFromXY(int x, int y) const {
    return {2.0f * static_cast<float>(x) / static_cast<float>(width) - 1.0f,
            -2.0f * static_cast<float>(y) / static_cast<float>(height) + 1.0f};
}

void Renderer::saveRender(const std::vector<int>& pixelValues, const std::string& filename) const {
    std::ofstream renderer{filename};
    renderer << "P3" << '\n';
    renderer << width << ' ' << height << '\n';
    renderer << 255 << '\n';

    for (int i : pixelValues) {
        renderer << i << ' ';
    }

    renderer.close();
}

void Renderer::setPixel(std::vector<int>& pixelValues, int row, int col, const Color& color) const {
    int redIndex{width * row * 3 + col * 3};
    pixelValues.at(redIndex) = convertTo8BitValue(color.r);
    pixelValues.at(redIndex + 1) = convertTo8BitValue(color.g);
    pixelValues.at(redIndex + 2) = convertTo8BitValue(color.b);
}

int Renderer::convertTo8BitValue(float f) { return static_cast<int>(std::round(f * 255)); }
