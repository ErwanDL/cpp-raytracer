#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <vector>

class Vector3;
struct Material;
class UniformIntervalRNG {
    mutable std::mt19937 generator;
    mutable std::uniform_real_distribution<float> distribution;

   public:
    UniformIntervalRNG(float min, float max);
    float generate() const;
};

class ReflectionGenerator {
    UniformIntervalRNG generator{-1.0f, 1.0f};
    int maxGlossyReflections;

    static float toAngle(float f, float exponent);

   public:
    ReflectionGenerator(int maxGlossyReflections);
    std::vector<Vector3> getRandomReflections(const Vector3 perfectReflection,
                                              const Material &material) const;
};

#endif