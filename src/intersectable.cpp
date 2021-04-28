#include "intersectable.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "sampling.hpp"
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
    return Intersection(intersectionLocation, normal, t, material);
}

PointSamplingResult Plane::sampleForDirectLighting(const Point3&) const {
    assert(false && "Not implemented yet");
    return PointSamplingResult(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 0.0f);
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
                        material);
}

PointSamplingResult Sphere::sampleForDirectLighting(const Point3& location) const {
    Vector3 centerToLocation = location - center;
    float dToCenter = centerToLocation.length();
    float cosThetaMax = radius / dToCenter;

    DirectionSamplingResult sample =
        sampleHemisphereCosineWeighted(centerToLocation.normalized(), cosThetaMax);
    Point3 point = center + radius * sample.direction;
    Vector3 normal = sample.direction;

    // PDF must be divided by R² since we are not on the unit sphere anymore
    return PointSamplingResult(point, normal, sample.pdf / Utils::sqr(radius));
}