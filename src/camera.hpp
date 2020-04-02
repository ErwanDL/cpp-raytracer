#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "vectors.hpp"

struct Ray;

class Camera {
   public:
    virtual Ray makeRay(const Point2 &point) const = 0;
    virtual const Point3 &getLocation() const = 0;
};

class PerspectiveCamera : public Camera {
   private:
    Point3 location;
    Vector3 forward;
    Vector3 up;
    Vector3 right;

    float height;
    float width;

   public:
    PerspectiveCamera(const Point3 &location, const Vector3 &target,
                      const Vector3 &upguide, float fov, float aspectRatio);
    Ray makeRay(const Point2 &point) const override;

    const Point3 &getLocation() const override;
};

#endif