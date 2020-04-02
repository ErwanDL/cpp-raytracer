#include "ray.hpp"

#include <algorithm>
#include <cassert>

#include "material.hpp"
#include "renderer.hpp"

// CLASS INTERSECTION

Intersection::Intersection(Point3 location, const Vector3 &normal,
                           float distanceToRayOrigin, const Material &material)
    : location(location),
      normal(normal.normalized()),
      distanceToRayOrigin(distanceToRayOrigin),
      material(material) {}

// CLASS RAY

Ray::Ray(const Point3 &origin, const Vector3 &direction, float maxDist)
    : origin(origin), direction(direction.normalized()), maxDist(maxDist) {}

bool Ray::isValidRayDistance(float t) const {
    return t > MIN_RAY_DIST && t < maxDist;
}
