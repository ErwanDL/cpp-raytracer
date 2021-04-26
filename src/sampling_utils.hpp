#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include "utils.hpp"
#include "vector3.hpp"
#include <cmath>
#include <utility>

namespace Utils {
/* Returns the given normalized vector rotated by polar angle theta and azimuth phi in its local
spherical coordinate system. The reference for the azimuth is chosen at random, as all my sampling
needs are isotropical for now. */
inline Vector3 sphericalCoordsRotation(const Vector3& zenithDirection, float theta, float phi) {
    // One of the many ways to get an orthogonal basis from the zenith direction.
    auto u = Vector3(-zenithDirection.y, zenithDirection.x, 0.0f).normalized();
    auto v = zenithDirection.cross(u);
    float sinTheta = std::sin(theta);

    return std::cos(theta) * zenithDirection + sinTheta * std::cos(phi) * u +
           sinTheta * std::sin(phi) * v;
}

/* The exponent is applied to the cosine of the polar angle. */
inline Vector3 sampleHemisphereDirection(const Vector3& zenithDirection, float exponent) {

    float theta = std::acos(std::pow(Utils::random(), exponent));
    float phi = Utils::TWO_PI * Utils::random();

    return sphericalCoordsRotation(zenithDirection, theta, phi);
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

inline std::pair<float, float> samplePixel(int x, int y) {
    return {x + Utils::random() - 0.5f, y + Utils::random() - 0.5f};
}
} // namespace Utils

#endif