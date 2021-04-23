#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include "utils.hpp"
#include "vector3.hpp"
#include <cmath>

inline Vector3 sampleHemisphere(const Vector3& zenithDirection, float smoothness) {
    // One of the many ways to get an orthogonal basis from the zenith direction.
    auto u = Vector3(-zenithDirection.y, zenithDirection.x, 0.0f).normalized();
    auto v = zenithDirection.cross(u);

    // polar angle, with the smoothness exponent applied to the cosine distribution
    float theta = std::acos(std::pow(Math::random(), 1.0f / smoothness));
    float phi = 2.0f * Math::PI * Math::random(); // azimuth, with u as the reference for 0 azimuth

    return std::cos(theta) * zenithDirection + std::sin(theta) * std::cos(phi) * u +
           std::sin(theta) * std::sin(phi) * v;
}
#endif