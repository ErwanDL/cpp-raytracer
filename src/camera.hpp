#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"
#include "vector3.hpp"
#include <cmath>

class PerspectiveCamera {
  private:
    Point3 location;
    Vector3 forward;
    Vector3 right;
    Vector3 up;
    float maxV;

  public:
    PerspectiveCamera(const Point3& location, const Point3& target, float vfov)
        : location(location), forward((target - location).normalized()),
          right(forward.cross(Vector3(0.0f, 1.0f, 0.0f)).normalized()), up(right.cross(forward)),
          maxV(std::tan(vfov / 2)) {}

    Ray makeRay(float x, float y, int width, int height) const {
        float u = 2.0f * (x - static_cast<float>(width) / 2.0f) / height;
        float v = 2.0f * (static_cast<float>(height) / 2.0f - y) / height;
        Vector3 direction{forward + u * maxV * right + v * up * maxV};
        return Ray(location, direction.normalized());
    }
};

#endif