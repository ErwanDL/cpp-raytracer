#include "camera.hpp"

#include <cmath>

#include "ray.hpp"
#include "vectors.hpp"

PerspectiveCamera::PerspectiveCamera(const Point3 &location,
                                     const Vector3 &target,
                                     const Vector3 &upguide, float fov,
                                     float aspectRatio)
    : location(location),
      forward((target - location).normalized()),
      up(0.0f),
      right(0.0f),
      height(std::tan(fov)),
      width(0.0f) {
    right = forward.cross(upguide).normalized();
    up = right.cross(forward);  // upguide is not necessarily colinear to up
    width = height * aspectRatio;
}

Ray PerspectiveCamera::makeRay(const Point2 &point) const {
    Vector3 direction{forward + point.getU() * width * right +
                      point.getV() * height * up};
    return Ray(location, direction);
}

const Point3 &PerspectiveCamera::getLocation() const { return location; }