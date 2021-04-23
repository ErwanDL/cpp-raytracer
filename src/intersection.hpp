#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "material.hpp"
#include "vector3.hpp"

struct Intersection {
    Point3 location;
    Vector3 normal;
    float distanceToRayOrigin;
    Material material;

    Intersection(Point3 location, const Vector3& normal, float distanceToRayOrigin,
                 const Material& material)
        : location(location), normal(normal), distanceToRayOrigin(distanceToRayOrigin),
          material(material) {}
};

#endif