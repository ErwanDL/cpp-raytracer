#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "material.hpp"
#include "vector3.hpp"
#include <functional>

struct Intersection {
    Point3 location;
    Vector3 normal;
    float distanceToRayOrigin;
    std::reference_wrapper<const Material> material;

    Intersection(Point3 location, const Vector3& normal, float distanceToRayOrigin,
                 const Material& material)
        : location(location), normal(normal), distanceToRayOrigin(distanceToRayOrigin),
          material(std::ref(material)) {}
};

#endif