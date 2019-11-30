#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vectors.hpp"

class Ray;

class Camera {
   public:
    virtual Ray makeRay(const Point2 &point) const = 0;
    virtual ~Camera() {}
};

class PerspectiveCamera : public Camera {
   private:
    Point3 origin{};
    Vector3 forward{};
    Vector3 up{};
    Vector3 right{};

    float height;
    float width;

   public:
    PerspectiveCamera(const Point3 &origin, const Vector3 &target,
                      const Vector3 &upguide, float fov, float aspectRatio);
    virtual ~PerspectiveCamera() override {}
    virtual Ray makeRay(const Point2 &point) const override;
};

#endif