#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include "utils.hpp"
#include "vector3.hpp"
#include <cmath>

/* The exponent is applied to the cosine of the polar angle. */
inline Vector3 sampleHemisphereDirection(const Vector3& zenithDirection, float exponent) {
    // One of the many ways to get an orthogonal basis from the zenith direction.
    auto u = Vector3(-zenithDirection.y, zenithDirection.x, 0.0f).normalized();
    auto v = zenithDirection.cross(u);

    float theta = std::acos(std::pow(Utils::random(), exponent));
    float phi =
        2.0f * Utils::PI * Utils::random(); // azimuth, with u as the reference for 0 azimuth

    return std::cos(theta) * zenithDirection + std::sin(theta) * std::cos(phi) * u +
           std::sin(theta) * std::sin(phi) * v;
}

inline Point3 sampleSpherePoint(const Point3& center, float radius) {
    // Using a rejection technique as it is likely more efficient in 3D than sampling spherical
    // coordinates, which requires extensive trigonometric and cube-root operations.
    float x, y, z;

    do {
        x = 2.0f * Utils::random() - 1.0f;
        y = 2.0f * Utils::random() - 1.0f;
        z = 2.0f * Utils::random() - 1.0f;
    } while (x * x + y * y + z * z > 1.0f);

    return Point3(center.x + radius * x, center.y + radius * y, center.z + radius * z);
}
#endif