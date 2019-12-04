#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "material.hpp"
#include "vectors.hpp"

class Ray;
class Scene;
class Camera;
class Light {
    Point3 origin{0.0f, 5.0f, -1.0f};
    Color color{0.5f};

   public:
    Light();
    Light(const Point3& origin, const Color& color);
    virtual ~Light() {}

    Color illuminate(const Ray& intersectedRay, const Scene& scene,
                     const Camera& cam) const;
};

#endif