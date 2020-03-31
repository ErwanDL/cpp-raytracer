#ifndef RAY_HPP
#define RAY_HPP

#include <vector>

#include "renderer.hpp"
#include "vectors.hpp"

class Shape;
class Intersection {
   public:
    constexpr static float MIN_RAY_DIST{0.001f};
    constexpr static float MAX_RAY_DIST{1.0e30f};

   private:
    float distance;
    Vector3 normal{0.0f};
    const Shape *pShape{nullptr};

   public:
    explicit Intersection(float distance = MAX_RAY_DIST);
    Intersection(float distance, const Vector3 &normal, const Shape *pShape);

    bool intersected() const;

    float getDistance() const;
    const Vector3 &getNormal() const;
    const Shape *getPShape() const;

    void setDistance(float newDistance);
    void setNormal(const Vector3 &newNormal);
    void setPShape(const Shape *newPShape);
};

class Ray {
   private:
    Point3 origin;
    Vector3 direction;
    Intersection intersection;

   public:
    Ray(const Point3 &origin, const Vector3 &direction,
        float maxdist = Intersection::MAX_RAY_DIST);

    Point3 pointAtDistance(float t) const;
    Point3 pointOfIntersection() const;

    const Point3 &getOrigin() const;
    const Vector3 &getDirection() const;
    const Intersection &getIntersection() const;
    Intersection &getIntersection();

    void setOrigin(const Point3 &newOrigin);
    void setDirection(const Vector3 &newDirection);
    void setIntersection(const Intersection &newIntersection);
};

#endif