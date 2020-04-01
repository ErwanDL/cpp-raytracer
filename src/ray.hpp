#ifndef RAY_HPP
#define RAY_HPP

#include <vector>

#include "renderer.hpp"
#include "vectors.hpp"

class Shape;

class Ray {
   public:
    constexpr static float MIN_RAY_DIST{0.001f};
    constexpr static float MAX_RAY_DIST{1.0e30f};

   private:
    Point3 origin;
    Vector3 direction;

   public:
    Ray(const Point3 &origin, const Vector3 &direction,
        float maxDist = MAX_RAY_DIST);
    float maxDist;

    Point3 pointAtDistance(float t) const;
    Point3 pointOfIntersection() const;

    const Point3 &getOrigin() const;
    const Vector3 &getDirection() const;

    void setOrigin(const Point3 &newOrigin);
    void setDirection(const Vector3 &newDirection);
};
struct Intersection {
    Point3 location;
    Vector3 normal{0.0f};
    const Shape *pShape;
    const Ray *pRay;

    Intersection(Point3 location, const Vector3 &normal, const Shape &shape,
                 const Ray &ray);

    const Shape &getShape() const;
};

#endif