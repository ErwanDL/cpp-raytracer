#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

class UniformIntervalRNG {
    mutable std::mt19937 generator;
    mutable std::uniform_real_distribution<float> distribution;

   public:
    UniformIntervalRNG(float min, float max);
    float generate() const;
};

#endif