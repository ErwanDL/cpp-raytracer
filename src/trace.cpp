#include "trace.hpp"
#include "camera.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include "scene.hpp"
#include "utils.hpp"
#include <chrono>
#include <cmath>

void displayProgress(float progressRatio) {
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

Color shootRayRecursively(const Ray& ray, const Scene& scene, int nBounces, int nSamples) {
    const auto intersection = scene.findFirstIntersection(ray);
    if (!intersection) {
        std::cout << nBounces << std::endl;
        return scene.skyColor;
    }
    Color intersectionColor = scene.computeTotalLighting(intersection.value(), ray.origin);
    const Material& material = intersection->material;

    if (nBounces == 0 || !material.metal) {
        return intersectionColor;
    }

    const Vector3 perfectReflectionDirection = ray.direction.reflected(intersection->normal);

    Color reflectedColor{0.0f};
    for (int i = 0; i < nSamples; ++i) {
        Vector3 sampledDirection =
            sampleHemisphere(perfectReflectionDirection, material.smoothness);
        // Reflected rays that would shoot beneath the surface are reflected about the
        // perfect reflection direction, back above the surface
        if (sampledDirection.dot(intersection->normal) < 0.0f) {
            sampledDirection = (-sampledDirection).reflected(perfectReflectionDirection);
        }
        const Ray reflectedRay{intersection->location, sampledDirection};
        reflectedColor += shootRayRecursively(reflectedRay, scene, nBounces - 1, nSamples);
    }
    reflectedColor /= nSamples;

    return intersectionColor +
           reflectedColor * (material.metal ? material.albedo : material.specularity);
}

Color gammaCorrect(const Color& color, float gamma) {
    return Color(Math::clamp(std::pow(color.r, 1.0f / gamma)),
                 Math::clamp(std::pow(color.g, 1.0f / gamma)),
                 Math::clamp(std::pow(color.b, 1.0f / gamma)));
}

std::vector<std::vector<Color>> rayTrace(const PerspectiveCamera& camera, const Scene& scene,
                                         const RenderParams& params) {
    std::vector<std::vector<Color>> render;

    std::cout << "Starting render...\n";
    const auto start = std::chrono::steady_clock::now();

    for (int y = 0; y < params.height; ++y) {
        std::vector<Color> line;
        displayProgress(static_cast<float>(y) / static_cast<float>(params.height));
        for (int x = 0; x < params.width; ++x) {
            Ray initialRay = camera.makeRay(x, y, params.width, params.height);
            auto color = shootRayRecursively(initialRay, scene, params.nBounces, params.nSamples);
            line.push_back(gammaCorrect(color, params.gamma));
        }
        render.push_back(line);
    }

    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
    std::cout << "\nScene rendered in " << static_cast<float>(duration.count()) / 1000.0f
              << " seconds.\n";

    return render;
}
