#ifndef RAY_HPP
#define RAY_HPP

#include <vector>

#include "vectors.hpp"

struct Ray {
    constexpr static float MIN_RAY_DIST{0.001f};
    constexpr static float MAX_RAY_DIST{1.0e30f};

    Point3 origin;
    Vector3 direction;
    float maxDist;

    Ray(const Point3& origin, const Vector3& direction, float maxDist = MAX_RAY_DIST);

    bool isValidRayDistance(float t) const;
};
#endif