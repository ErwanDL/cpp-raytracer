#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <cmath>
#include <optional>
#include <random>
#include <utility>

namespace Math {
template <typename T> T sqr(T x) { return x * x; }
constexpr float PI{3.141592f};

/* Returns a random float uniformly between 0.0f and 1.0f. */
inline float random() {
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float signBitToNumber(bool signBit) { return signBit ? 1.0f : -1.0f; }

inline bool floatingPointEquality(float a, float b) {
    // Check if the numbers are really close -- needed when comparing numbers
    // near zero.
    constexpr float absEpsilon{1e-6f};
    constexpr float relEpsilon{1e-12f};

    float diff{std::abs(a - b)};
    if (diff <= absEpsilon) {
        return true;
    }

    // Otherwise fall back to Knuth's algorithm
    return (diff <= (std::max(std::abs(a), std::abs(b)) * relEpsilon));
}

inline float clamp(float x, float min = 0.0f, float max = 1.0f) { return std::clamp(x, min, max); }

/*
 * Returns false if the equation has no solution, returns a pair (t1, t2) with
 * t1 <= t2 else.
 */
inline std::optional<std::pair<float, float>> solveSecondDegreeEquation(float a, float b, float c) {
    float discriminant{Math::sqr(b) - 4 * a * c};

    if (discriminant < 0.0f) {
        return {};
    }

    float t1{(-b - std::sqrt(discriminant)) / (2 * a)};
    float t2{(-b + std::sqrt(discriminant)) / (2 * a)};
    return std::make_pair(t1, t2);
}
} // namespace Math

#endif