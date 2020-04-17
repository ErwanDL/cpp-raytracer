#include "random.hpp"

#include <random>

UniformIntervalRNG::UniformIntervalRNG(float min, float max)
    : generator(std::random_device{}()), distribution(min, max) {}

float UniformIntervalRNG::generate() const { return distribution(generator); }