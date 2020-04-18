#ifndef UTILS_HPP
#define UTILS_HPP
#include <algorithm>
#include <cmath>
#include <optional>
#include <utility>

namespace Math {
template <typename T>
T sqr(T x) {
    return x * x;
}
constexpr float PI{3.141592f};

inline float signBitToNumber(bool signBit) { return signBit ? 1.0f : -1.0f; }

template <typename T>
bool floatingPointEquality(T a, T b) {
    // Check if the numbers are really close -- needed when comparing numbers
    // near zero.
    constexpr double absEpsilon{1e-6};
    constexpr double relEpsilon{1e-12};

    float diff{std::abs(a - b)};
    if (diff <= absEpsilon) {
        return true;
    }

    // Otherwise fall back to Knuth's algorithm
    return (diff <= (std::max(std::abs(a), std::abs(b)) * relEpsilon));
}

template <typename T>
T unitClamp(T x) {
    return std::clamp(x, static_cast<T>(0), static_cast<T>(1));
}

/*
 * Returns false if the equation has no solution, returns a pair (t1, t2) with
 * t1 <= t2 else.
 */
inline std::optional<std::pair<float, float>> solveSecondDegreeEquation(
    float a, float b, float c) {
    float discriminant{Math::sqr(b) - 4 * a * c};

    if (discriminant < 0.0f) {
        return {};
    }

    float t1{(-b - std::sqrt(discriminant)) / (2 * a)};
    float t2{(-b + std::sqrt(discriminant)) / (2 * a)};
    return std::make_pair(t1, t2);
}
}  // namespace Math

#endif