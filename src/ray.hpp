#ifndef RAY_HPP
#define RAY_HPP

#include "vector3.hpp"

struct Ray {
    constexpr static float MIN_RAY_DIST{0.001f};
    constexpr static float MAX_RAY_DIST{1.0e4f};

    Point3 origin;
    Vector3 direction;
    float maxDist = MAX_RAY_DIST;
    bool isSpecular;

    Ray(const Point3& origin, const Vector3& direction, bool isSpecular = false)
        : origin(origin), direction(direction.normalized()), isSpecular(isSpecular) {}

    bool isValidRayDistance(float t) const { return t > MIN_RAY_DIST && t < maxDist; }
};
#endif