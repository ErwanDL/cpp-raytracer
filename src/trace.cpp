#include "trace.hpp"
#include "camera.hpp"
#include "params.hpp"
#include "ray.hpp"
#include "sampling.hpp"
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
    float exponent = 1.0f / gamma;
    return Color(std::pow(color.r, exponent), std::pow(color.g, exponent),
                 std::pow(color.b, exponent));
}

std::vector<std::vector<Color>> rayTrace(const PerspectiveCamera& camera, const Scene& scene,
                                         const RenderParams& params) {
    std::vector<std::vector<Color>> render(params.height);

#if defined(_OPENMP)
    // This may need tweaking for optimal performance depending on your machine :
    // I noticed that using hyperthreading (all 8 logical cores) resulted in 10-15%
    // worse performance on my machine than just using the 4 physical cores.
    const int num_threads = omp_get_max_threads() / 2;
    omp_set_num_threads(num_threads);
#else
    const int num_threads = 1;
#endif

    std::cout << "Starting render on " << num_threads << " threads..." << std::endl;
    const auto start = std::chrono::steady_clock::now();

    for (int y = 0; y < params.height; ++y) {
        std::vector<Color> line(params.width);
        std::string bar = progressBar(static_cast<float>(y) / static_cast<float>(params.height));
        std::cout << '\r' << bar << std::flush;

#if defined(_OPENMP)
#pragma omp parallel for schedule(guided)
#endif
        for (int x = 0; x < params.width; ++x) {
            Color pixelColor{0.0f};
            for (int i = 0; i < params.nSamples; ++i) {
                auto sampledPixel = Utils::randomOffsetPixel(x, y); // to prevent aliasing
                Ray initialRay = camera.makeRay(sampledPixel.first, sampledPixel.second,
                                                params.width, params.height);
                pixelColor += scene.shootRay(initialRay, params.maxBounces, true);
            }
            pixelColor /= params.nSamples;
            line[x] = gammaCorrect((pixelColor).clamped(), params.gamma);
        }
        render[y] = line;
    }

    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start);
    std::cout << "\nScene rendered in " << static_cast<float>(duration.count()) / 1000.0f
              << " seconds.\n";

    return render;
}
