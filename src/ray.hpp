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
};

#endif