#include <optional>

#include "intersection.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "utils.hpp"

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

std::optional<Intersection> Sphere::intersect(const Ray& ray) const {
    // float a = raydirection.lengthSquared() is always equal to 1
    float a = 1.0f;
    float b = 2 * ray.direction.dot(ray.origin - centre);
    float c = (ray.origin - centre).lengthSquared() - Utils::sqr(radius);

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
    return Intersection(intersectionLocation, (intersectionLocation - centre).normalized(), t,
                        this->material);
}
