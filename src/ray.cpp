#include "ray.hpp"

#include <cassert>

#include "renderer.hpp"

// CLASS INTERSECTION

Intersection::Intersection(Point3 location, const Vector3 &normal,
                           const Shape &shape, const Ray &ray)
    : location(location), normal(normal), pShape(&shape), pRay(&ray) {}

const Shape &Intersection::getShape() const { return *pShape; }
// CLASS RAY

Ray::Ray(const Point3 &origin, const Vector3 &direction, float maxDist)
    : origin(origin), direction(direction), maxDist(maxDist) {}

Point3 Ray::pointAtDistance(float t) const { return origin + t * direction; }

const Point3 &Ray::getOrigin() const { return origin; }
const Vector3 &Ray::getDirection() const { return direction; }

void Ray::setOrigin(const Point3 &newOrigin) { origin = newOrigin; }
void Ray::setDirection(const Vector3 &newDirection) {
    direction = newDirection;
}