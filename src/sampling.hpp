#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include "utils.hpp"
#include "vector3.hpp"
#include <cmath>

/* The exponent is applied to the cosine of the polar angle. */
inline Vector3 sampleHemisphere(const Vector3& zenithDirection, float exponent) {
    // One of the many ways to get an orthogonal basis from the zenith direction.
    auto u = Vector3(-zenithDirection.y, zenithDirection.x, 0.0f).normalized();
    auto v = zenithDirection.cross(u);

    float theta = std::acos(std::pow(Utils::random(), exponent));
    float phi =
        2.0f * Utils::PI * Utils::random(); // azimuth, with u as the reference for 0 azimuth

    return std::cos(theta) * zenithDirection + std::sin(theta) * std::cos(phi) * u +
           std::sin(theta) * std::sin(phi) * v;
}
#endif