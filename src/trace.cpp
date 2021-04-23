#include "trace.hpp"
#include "camera.hpp"
#include "ray.hpp"
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

Color gammaCorrect(const Color& color, float gamma) {
    return Color(Utils::clamp(std::pow(color.r, 1.0f / gamma)),
                 Utils::clamp(std::pow(color.g, 1.0f / gamma)),
                 Utils::clamp(std::pow(color.b, 1.0f / gamma)));
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
            Color pixelColor{0.0f};
            for (int i = 0; i < params.nSamples; ++i) {
                Ray initialRay = camera.makeRay(static_cast<float>(x), static_cast<float>(y),
                                                params.width, params.height);
                pixelColor += scene.shootRay(initialRay, params.maxBounces);
            }
            line.push_back(gammaCorrect(pixelColor / params.nSamples, params.gamma));
        }
        render.push_back(line);
    }

    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
    std::cout << "\nScene rendered in " << static_cast<float>(duration.count()) / 1000.0f
              << " seconds.\n";

    return render;
}
