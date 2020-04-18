#include "renderer.hpp"

#include <cmath>
#include <exception>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "light.hpp"
#include "shape.hpp"
#include "utils.hpp"
#include "vectors.hpp"

Renderer::Renderer(const Intersectable &scene, const Light &lights, int width,
                   int height, const SupersamplingStrategy &superSampler,
                   float exposure, float gamma)
    : scene(scene),
      lights(lights),
      width(width),
      height(height),
      superSampler(superSampler),
      exposure(exposure),
      gamma(gamma) {}

std::vector<int> Renderer::rayTrace(const Camera &camera,
                                    int nReflexions) const {
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
                averageColor += shootRayRecursively(initialRay, nReflexions);
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
    const auto reflections =
        getRandomReflections(ray.direction.reflected(intersection->normal),
                             intersection->material.smoothness);

    Color reflectedColor{0.0f};
    int shotRays = 0;
    for (const Vector3 reflection : reflections) {
        if (reflection.dot(intersection->normal) > 0.0f) {
            const Ray reflectedRay{intersection->location, reflection};
            reflectedColor +=
                shootRayRecursively(reflectedRay, nReflexionsLeft - 1);
            shotRays += 1;
        }
    }

    return intersectionColor +
           intersection->material.specularColor *
               (reflectedColor / static_cast<float>(shotRays));
}

std::vector<Vector3> Renderer::getRandomReflections(const Vector3 mainDirection,
                                                    float smoothness) const {
    constexpr int nReflections = 100;
    std::vector<Vector3> reflections;
    for (int i = 0; i < nReflections; ++i) {
        const float theta = toAngle(generator.generate(), smoothness);
        const float phi = toAngle(generator.generate(), smoothness);
        reflections.push_back(
            Vector3::sphericallyRotated(mainDirection, theta, phi));
    }
    return reflections;
}

float Renderer::toAngle(float f, float exponent) {
    return Math::signBitToNumber(std::signbit(f)) *
           std::asin(std::pow(std::abs(f), exponent));
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

// CLASS DETERMINISTICSUPERSAMPLER

DeterministicSupersampler::DeterministicSupersampler(int rate) : rate(rate) {
    if (rate <= 0) {
        throw std::domain_error("Supersampling rate muste be >= 1");
    }
}

std::vector<std::pair<float, float>>
DeterministicSupersampler::getSupersamplingOffsets() const {
    const float unitSpacing = 0.5f / static_cast<float>(rate);
    std::vector<float> floatOffsets(rate);
    for (int i = 0; i < rate; ++i) {
        floatOffsets[i] =
            -0.5f + unitSpacing + 2.0f * static_cast<float>(i) * unitSpacing;
    }
    std::vector<std::pair<float, float>> vectorOffsets(rate * rate);
    int cursor = 0;
    for (const float u : floatOffsets) {
        for (const float v : floatOffsets) {
            vectorOffsets[cursor] = {u, v};
            ++cursor;
        }
    }
    return vectorOffsets;
}

// CLASS STOCHASTICSUPERSAMPLER

StochasticSupersampler::StochasticSupersampler(int samplesPerPixel)
    : samplesPerPixel(samplesPerPixel) {
    if (samplesPerPixel <= 0) {
        throw std::domain_error("Number of samples per pixel muste be >= 1");
    }
}

std::vector<std::pair<float, float>>
StochasticSupersampler::getSupersamplingOffsets() const {
    std::vector<std::pair<float, float>> vectorOffsets(samplesPerPixel);
    for (int i = 0; i < samplesPerPixel; ++i) {
        vectorOffsets[i] = {generator.generate(), generator.generate()};
    }
    return vectorOffsets;
}