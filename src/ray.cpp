#include "ray.hpp"
#include "image.hpp"

// CLASS INTERSECTION

Intersection::Intersection() {}

Intersection::Intersection(const Intersection &other)
    : distance(other.distance), normal(other.normal), pShape(other.pShape) {}
Intersection::Intersection(float distance, const Vector3 &normal,
                           const Shape *pShape)
    : distance(distance), normal(normal), pShape(pShape) {}

bool Intersection::intersected() const { return (pShape != nullptr); }

float Intersection::getDistance() const { return distance; }
const Vector3 &Intersection::getNormal() const { return normal; }
const Shape *Intersection::getPShape() const { return pShape; }

void Intersection::setDistance(float newDistance) { distance = newDistance; }
void Intersection::setNormal(const Vector3 &newNormal) { normal = newNormal; }
void Intersection::setPShape(const Shape *newPShape) { pShape = newPShape; }

Intersection &Intersection::operator=(const Intersection &other) {
    distance = other.distance;
    normal = other.normal;
    pShape = other.pShape;
    return *this;
}

// CLASS RAY

Ray::Ray(const Point3 &origin, const Vector3 &direction)
    : origin(origin), direction(direction) {}

Point3 Ray::pointAtDistance(float t) const {
    return Point3(origin + t * direction);
}

const Point3 &Ray::getOrigin() const { return origin; }
const Vector3 &Ray::getDirection() const { return direction; }
const Intersection &Ray::getIntersection() const { return intersection; }
Intersection &Ray::getIntersection() { return intersection; }

void Ray::setOrigin(const Point3 &newOrigin) { origin = newOrigin; }
void Ray::setDirection(const Vector3 &newDirection) {
    direction = newDirection;
}
void Ray::setIntersection(const Intersection &newIntersection) {
    intersection = newIntersection;
}