#include "ray.hpp"

Ray::Ray() {}
Ray::Ray(const Ray &r)
    : origin(r.origin), direction(r.direction), maxDist(r.maxDist) {}
Ray::Ray(const Point3 &origin, const Vector3 &direction, float maxDist)
    : origin(origin), direction(direction), maxDist(maxDist) {}

Point3 Ray::calculate(float t) const
{
    return Point3(origin + t * direction);
}

Point3 Ray::getOrigin() const { return origin; }
Vector3 Ray::getDirection() const { return direction; }
float Ray::getMaxDist() const { return maxDist; }

void Ray::setOrigin(Point3 newOrigin) { origin = newOrigin; }
void Ray::setDirection(Vector3 newDirection) { direction = newDirection; }
void Ray::setMaxDist(float newMaxDist) { maxDist = newMaxDist; }

Intersection::Intersection() {}
Intersection::Intersection(const Intersection &i)
    : ray(i.ray), t(i.t), pShape(i.pShape) {}
Intersection::Intersection(const Ray &ray) : ray(ray), t(ray.getMaxDist()) {}

bool Intersection::intersected() const
{
    return (pShape != nullptr);
}

Point3 Intersection::position() const
{
    return ray.calculate(t);
}