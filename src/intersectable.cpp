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

Color Plane::computeDirectDiffuse(const Point3&, const Point3&) const {
    assert(false && "Not implemented yet");
    return Color::BLACK;
}

Point3 Plane::sampleVisibleSurface(const Point3&) const {
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
                        material);
}

Color Sphere::computeDirectDiffuse(const Point3& location, const Point3& sampledPoint) const {
    Vector3 pointToLocation = location - sampledPoint;
    Vector3 normal = (sampledPoint - center).normalized();
    float cosThetaMax = radius / (center - location).length();
    float pdf = 1.0f / (Utils::TWO_PI * (1.0f - cosThetaMax) * Utils::sqr(radius));
    return normal.dot(pointToLocation.normalized()) * material.emission * material.color /
           (pdf * pointToLocation.lengthSquared());
}

Point3 Sphere::sampleVisibleSurface(const Point3& viewer) const {
    float dToCenter = (center - viewer).length();
    float cosThetaMax = radius / dToCenter;

    float phi = Utils::TWO_PI * Utils::random();
    // Uniform sampling of the visible portion of the hemisphere, delimited by angle thetaMax
    float theta = std::acos(1 - Utils::random() * (1 - cosThetaMax));

    Vector3 sampledDirection = sphericalCoordsRotation((viewer - center).normalized(), theta, phi);

    return center + radius * sampledDirection;
}