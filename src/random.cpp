#include "random.hpp"

#include <random>

#include "material.hpp"
#include "utils.hpp"
#include "vectors.hpp"

// UNIFORMINTERVALRNG

UniformIntervalRNG::UniformIntervalRNG(float min, float max)
    : generator(std::random_device{}()), distribution(min, max) {}

float UniformIntervalRNG::generate() const { return distribution(generator); }

// REFLECTIONGENERATOR
ReflectionGenerator::ReflectionGenerator(int maxGlossyReflections)
    : maxGlossyReflections(maxGlossyReflections) {}

std::vector<Vector3> ReflectionGenerator::getRandomReflections(
    const Vector3 perfectReflection, const Material &material) const {
    const int nRandomReflections = static_cast<int>(
        maxGlossyReflections * material.specularity() / (material.smoothness));

    if (material.smoothness > 20.0f) {
        return std::vector<Vector3>{perfectReflection};
    }

    std::vector<Vector3> reflections;
    for (int i = 0; i < nRandomReflections; ++i) {
        const float theta = toAngle(generator.generate(), material.smoothness);
        const float phi = toAngle(generator.generate(), material.smoothness);
        reflections.push_back(
            Vector3::sphericallyRotated(perfectReflection, theta, phi));
    }
    return reflections;
}

float ReflectionGenerator::toAngle(float f, float exponent) {
    return Math::signBitToNumber(std::signbit(f)) *
           std::asin(std::pow(std::abs(f), exponent));
}