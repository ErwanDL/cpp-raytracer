#include "trace.hpp"
#include "camera.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "utils.hpp"
#include <chrono>
#include <cmath>
#include <omp.h>
#include <sstream>
#include <string>

std::string progressBar(float progressRatio) {
    constexpr int nChars = 50;
    const int nSquares = static_cast<int>(std::round(progressRatio * nChars));
    std::ostringstream progressBar;
    for (int i = 0; i < nSquares; ++i) {
        progressBar << "■";
    }
    for (int i = nSquares; i < nChars; ++i) {
        progressBar << "-";
    }
    progressBar << ' ' << static_cast<int>(std::round(100 * progressRatio)) << '%';
    return progressBar.str();
}

Color gammaCorrect(const Color& color, float gamma) {
    return Color(Utils::clamp(std::pow(color.r, 1.0f / gamma)),
                 Utils::clamp(std::pow(color.g, 1.0f / gamma)),
                 Utils::clamp(std::pow(color.b, 1.0f / gamma)));
}

std::vector<std::vector<Color>> rayTrace(const PerspectiveCamera& camera, const Scene& scene,
                                         const RenderParams& params) {
    std::vector<std::vector<Color>> render(params.height);

    // This may need tweaking for optimal performance depending on your machine :
    // I noticed that using hyperthreading (all 8 logical cores) resulted in 10-15%
    // worse performance on my machine than just using the 4 physical cores.
    const int num_threads = omp_get_max_threads() / 2;
    omp_set_num_threads(num_threads);

    std::cout << "Starting render on " << num_threads << " threads..." << std::endl;
    const auto start = std::chrono::steady_clock::now();

    for (int y = 0; y < params.height; ++y) {
        std::vector<Color> line(params.width);
        std::string bar = progressBar(static_cast<float>(y) / static_cast<float>(params.height));
        std::cout << '\r' << bar << std::flush;

#pragma omp parallel for schedule(guided)
        for (int x = 0; x < params.width; ++x) {
            Color pixelColor{0.0f};
            for (int i = 0; i < params.nSamples; ++i) {
                Ray initialRay = camera.makeRay(static_cast<float>(x), static_cast<float>(y),
                                                params.width, params.height);
                pixelColor += scene.shootRay(initialRay, params.maxBounces);
            }
            line[x] = gammaCorrect(pixelColor / params.nSamples, params.gamma);
        }
        render[y] = line;
    }

    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
    std::cout << "\nScene rendered in " << static_cast<float>(duration.count()) / 1000.0f
              << " seconds.\n";

    return render;
}
