#ifndef SUPERSAMPLING_HPP
#define SUPERSAMPLING_HPP

#include <vector>

#include "random.hpp"

class SupersamplingStrategy {
   public:
    virtual std::vector<std::pair<float, float>> getSupersamplingOffsets()
        const = 0;
};

class DeterministicSupersampler : public SupersamplingStrategy {
    int rate;

   public:
    explicit DeterministicSupersampler(int rate);
    std::vector<std::pair<float, float>> getSupersamplingOffsets()
        const override;
};

class StochasticSupersampler : public SupersamplingStrategy {
    int samplesPerPixel;
    UniformIntervalRNG generator{-0.5f, 0.5f};

   public:
    explicit StochasticSupersampler(int samplesPerPixel);
    std::vector<std::pair<float, float>> getSupersamplingOffsets()
        const override;
};
#endif