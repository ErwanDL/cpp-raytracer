#include "ray.hpp"

#include <algorithm>
#include <cassert>

#include "material.hpp"
#include "renderer.hpp"

// CLASS INTERSECTION

Intersection::Intersection(Point3 location, const Vector3 &normal,
                           const Material &material)
    : location(location), normal(normal.normalized()), material(material) {}

// CLASS RAY

Ray::Ray(const Point3 &origin, const Vector3 &direction, float maxDist)
    : origin(origin), direction(direction.normalized()), maxDist(maxDist) {}
