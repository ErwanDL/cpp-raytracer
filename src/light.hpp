#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "material.hpp"
#include "vector3.hpp"

struct Scene;
struct Intersection;
class Intersectable;
class Camera;

class Light {
  public:
    virtual Color illuminate(const Intersection& intersection, const Scene& scene,
                             const Point3& observerLocation) const = 0;
};

class SphereLight : public Light {
    Point3 origin;
    Color color;
    float intensity;
    float radius;

  public:
    SphereLight(const Point3& origin, const Color& color, float intensity, float radius = 0.25f)
        : origin(origin), color(color), intensity(intensity), radius(radius) {}

    Color illuminate(const Intersection& intersection, const Scene& scene,
                     const Point3& observerLocation) const override;

  private:
    Color computeDiffuse(float lightDotN, const Material& material) const;
    Color computeSpecular(float observerDotReflected, const Material& material) const;
};

#endif