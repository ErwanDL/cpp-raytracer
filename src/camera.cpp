#include "camera.hpp"
#include <cmath>
#include "ray.hpp"
#include "vectors.hpp"

Camera::Camera(const Point3 &origin,
                                     const Vector3 &target,
                                     const Vector3 &upguide, float fov,
                                     float aspectRatio)
    : origin(origin),
      forward((target - origin).normalized()),
      height(std::tan(fov)) {
    right = forward.cross(upguide).normalized();
    up = right.cross(forward);
    width = height * aspectRatio;
}

Ray Camera::makeRay(const Point2 &point) const {
    Vector3 direction{forward + point.getU() * width * right +
                      point.getV() * height * up};
    direction.normalize();
    return Ray(origin, direction);
}

const Point3 &Camera::getOrigin() const { return origin; }