#include "intersectable.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "sampling_utils.hpp"
#include "utils.hpp"
#include <cmath>
#include <optional>

std::optional<Intersection> Plane::intersect(const Ray& ray) const {
    float dDotN{normal.dot(ray.direction)};

    if (dDotN == 0.0f) {
        return {};
    }

    const float t = normal.dot(position - ray.origin) / dDotN;

    if (!ray.isValidRayDistance(t)) {
        return {};
    }

    Point3 intersectionLocation = ray.origin + t * ray.direction.normalized();
    Vector3 intersectionNormal = dDotN > 0.0f ? -normal : normal;
    return Intersection(intersectionLocation, intersectionNormal, t, this->material);
}

Color Plane::computeDirectDiffuse(const Point3&, const Point3&) const {
    assert(false && "Not implemented yet");
    return Color::BLACK;
}

Point3 Plane::sampleSolidAngle(const Point3&) const {
    assert(false && "Not implemented yet");
    return Point3(0.0f, 0.0f, 0.0f);
}

std::optional<Intersection> Sphere::intersect(const Ray& ray) const {
    // float a = raydirection.lengthSquared() is always equal to 1
    float a = 1.0f;
    float b = 2 * ray.direction.dot(ray.origin - center);
    float c = (ray.origin - center).lengthSquared() - Utils::sqr(radius);

    auto solutions = Utils::solveSecondDegreeEquation(a, b, c);

    if (!solutions) {
        return {};
    }

    float t;
    // since t1 always <= t2, we check t1 first
    if (ray.isValidRayDistance(solutions->first)) {
        t = solutions->first;
    } else if (ray.isValidRayDistance(solutions->second)) {
        t = solutions->second;
    } else {
        return {};
    }

    Point3 intersectionLocation = ray.origin + t * ray.direction.normalized();
    return Intersection(intersectionLocation, (intersectionLocation - center).normalized(), t,
                        this->material);
}

Color Sphere::computeDirectDiffuse(const Point3& location, const Point3& sampledPoint) const {
    float cosThetaMax = (sampledPoint - location).length() / (center - location).length();
    // Solid angle of the cone from location to the sphere
    float normalization = 1.0f - cosThetaMax;
    return material.emission * material.color * normalization;
}

Point3 Sphere::sampleSolidAngle(const Point3& location) const {
    float d = (center - location).length();
    float sinThetaMax = radius / d;
    float phi = Utils::TWO_PI * Utils::random();
    float cosThetaMax = std::sqrt(1 - Utils::sqr(sinThetaMax));

    // Sampling theta : not just random() * sinThetaMax like I thought before !
    float theta = std::acos(1 - Utils::random() * (1 - cosThetaMax));
    Vector3 directionToSample =
        Utils::sphericalCoordsRotation((center - location).normalized(), theta, phi);

    // Using Al-Kashi's theorem, and clamping x's value beforehand to avoid floating point
    // errors resulting in the square root returning NaN
    float x = Utils::sqr(radius) - Utils::sqr(std::sin(theta) * d);
    float distToSample = d * std::cos(theta) - std::sqrt(std::max(0.0f, x));

    return location + distToSample * directionToSample;
}