#ifndef RAY_HPP
#define RAY_HPP

#include "vector3.hpp"

class Ray
{
public:
    constexpr static float MIN_RAY_DIST{0.001f};
    constexpr static float MAX_RAY_DIST{1.0e30f};

private:
    Point3 origin{};
    Vector3 direction{};
    float maxDist{MAX_RAY_DIST};

public:
    Ray();
    Ray(const Ray &r);
    Ray(const Point3 &origin, const Vector3 &direction, float maxDist = MAX_RAY_DIST);

    Point3 calculate(float t) const;

    Point3 getOrigin() const;
    Vector3 getDirection() const;
    float getMaxDist() const;

    void setOrigin(Point3 newOrigin);
    void setDirection(Vector3 newDirection);
    void setMaxDist(float newMaxDist);
};

class Shape;
class Intersection
{
    Ray ray{};
    float t{Ray::MAX_RAY_DIST};
    Shape *pShape{nullptr};

    Intersection();
    Intersection(const Intersection &i);
    Intersection(const Ray &ray);

    bool intersected() const;
    Point3 position() const;
};

#endif