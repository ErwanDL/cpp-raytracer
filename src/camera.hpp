#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "vectors.hpp"

class Ray;

class Camera {
   private:
    Point3 origin{};
    Vector3 forward{};
    Vector3 up{};
    Vector3 right{};

    float height{1.0f};
    float width{1.0f};

   public:
    Camera(const Point3 &origin, const Vector3 &target, const Vector3 &upguide,
           float fov, float aspectRatio);
    virtual ~Camera() {}
    virtual Ray makeRay(const Point2 &point) const;

    const Point3 &getOrigin() const;
};

#endif