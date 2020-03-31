#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "vectors.hpp"

class Ray;

class Camera {
   public:
    virtual ~Camera() = default;
    virtual Ray makeRay(const Point2 &point) const = 0;
    virtual const Point3 &getOrigin() const = 0;
};

class PerspectiveCamera : public Camera {
   private:
    Point3 origin;
    Vector3 forward;
    Vector3 up;
    Vector3 right;

    float height{1.0f};
    float width{1.0f};

   public:
    PerspectiveCamera(const Point3 &origin, const Vector3 &target,
                      const Vector3 &upguide, float fov, float aspectRatio);
    ~PerspectiveCamera() override = default;
    Ray makeRay(const Point2 &point) const override;

    const Point3 &getOrigin() const override;
};

#endif