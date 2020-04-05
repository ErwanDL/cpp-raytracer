#include "ray.hpp"

#include <algorithm>
#include <cassert>

#include "material.hpp"
#include "renderer.hpp"

// CLASS INTERSECTION

Intersection::Intersection(Point3 location, const Vector3 &normal,
                           float distanceToRayOrigin, const Material &material,
                           const Shape &intersectedShape)
    : location(location),
      normal(normal.normalized()),
      distanceToRayOrigin(distanceToRayOrigin),
      material(material),
      intersectedShape(&intersectedShape) {}

Intersection &Intersection::operator=(const Intersection &other) {
    if (&other == this) {
        return *this;
    }
    location = other.location;
    normal = other.normal;
    distanceToRayOrigin = other.distanceToRayOrigin;
    material = other.material;
    intersectedShape = other.intersectedShape;
    return *this;
}

// CLASS RAY

Ray::Ray(const Point3 &origin, const Vector3 &direction, float maxDist)
    : origin(origin), direction(direction.normalized()), maxDist(maxDist) {}

bool Ray::isValidRayDistance(float t) const {
    return t > MIN_RAY_DIST && t < maxDist;
}
