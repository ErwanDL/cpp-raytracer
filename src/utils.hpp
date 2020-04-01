#ifndef UTILS_HPP
#define UTILS_HPP
#include <algorithm>
#include <cmath>

namespace Math {
template <typename T>
T sqr(T x) {
    return x * x;
}
constexpr float PI{3.141592f};

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
}  // namespace Math

#endif