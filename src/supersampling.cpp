#include "supersampling.hpp"

#include <exception>
#include <vector>
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