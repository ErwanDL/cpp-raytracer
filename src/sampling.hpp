#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include "utils.hpp"
#include "vector3.hpp"
#include <cmath>

struct DirectionSamplingResult {
    Vector3 direction;
    float pdf;

    DirectionSamplingResult(const Vector3& direction, float pdf) : direction(direction), pdf(pdf) {}
};

struct PointSamplingResult {
    Point3 point;
    Vector3 normal;
    float pdf;

    PointSamplingResult(const Point3& point, Vector3 normal, float pdf)
        : point(point), normal(normal), pdf(pdf) {}
};

/* Returns the given normalized vector rotated by polar angle theta and azimuth phi in its local
spherical coordinate system. The reference for the azimuth is chosen at random, as all my
sampling needs are isotropical for now. */
inline Vector3 sphericalCoordsRotation(const Vector3& zenithDirection, float theta, float phi) {
    // One of the many ways to get an orthogonal basis from the zenith direction.
    float x = zenithDirection.x, y = zenithDirection.y, z = zenithDirection.z;
    auto u = (x != 0.0f || y != 0.0f) ? Vector3(-y, x, 0.0f).normalized()
                                      : Vector3(-z, 0.0f, x).normalized();
    auto v = zenithDirection.cross(u);

    float sinTheta = std::sin(theta);
    return std::cos(theta) * zenithDirection + sinTheta * std::cos(phi) * u +
           sinTheta * std::sin(phi) * v;
}

/* EDIT: This is actually a bizarre method and does not produce a cosine-weighted sampling,
   like I initially thought it did. */
inline Vector3 sampleHemisphereGlossy(const Vector3& zenithDirection, float exponent) {
    float theta = std::acos(std::pow(Utils::random(), exponent));
    float phi = Utils::TWO_PI * Utils::random();

    Vector3 dir = sphericalCoordsRotation(zenithDirection, theta, phi);
    return dir;
}

/* The proper way of doing cosine-weighted hemisphere sampling.
   You can sample only a portion of the hemisphere by providing the cosine
   of the maximum polar angle.
   Default value is 1 (i.e. an angle of PI/2), which corresponds to the whole hemisphere. */
inline DirectionSamplingResult sampleHemisphereCosineWeighted(const Vector3& zenithDirection,
                                                              float cosThetaMax = 0.0f) {
    float sinThetaMaxSquared = 1 - Utils::sqr(cosThetaMax);
    float theta = std::asin(std::sqrt(Utils::random() * sinThetaMaxSquared));
    float phi = Utils::TWO_PI * Utils::random();
    float pdf = std::cos(theta) / (Utils::PI * sinThetaMaxSquared);

    Vector3 dir = sphericalCoordsRotation(zenithDirection, theta, phi);
    return DirectionSamplingResult(dir, pdf);
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