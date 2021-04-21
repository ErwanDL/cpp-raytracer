#include "ray.hpp"

#include <algorithm>
#include <cassert>

struct Vector3;

// CLASS RAY

Ray::Ray(const Point3& origin, const Vector3& direction, float maxDist)
    : origin(origin), direction(direction.normalized()), maxDist(maxDist) {}

bool Ray::isValidRayDistance(float t) const { return t > MIN_RAY_DIST && t < maxDist; }
