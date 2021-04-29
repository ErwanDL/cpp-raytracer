#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "vector3.hpp"
#include <functional>

struct Material;

struct Intersection {
    Point3 location;
    Vector3 normal;
    float distanceToRayOrigin;
    std::reference_wrapper<const Material> material;
    bool backFace;

    Intersection(Point3 location, const Vector3& normal, float distanceToRayOrigin,
                 const Material& material, bool backFace)
        : location(location), normal(normal), distanceToRayOrigin(distanceToRayOrigin),
          material(std::ref(material)), backFace(backFace) {}
};

#endif