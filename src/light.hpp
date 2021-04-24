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

class AmbientLight : public Light {
    Color color;

  public:
    AmbientLight(const Color& color) : color(color) {}

    Color illuminate(const Intersection& intersection, const Scene& scene,
                     const Point3& observerLocation) const override;
};

class SphereLight : public Light {
    Point3 origin;
    Color color;
    float radius;

  public:
    SphereLight(const Point3& origin, const Color& color, float radius)
        : origin(origin), color(color), radius(radius) {}

    Color illuminate(const Intersection& intersection, const Scene& scene,
                     const Point3& observerLocation) const override;

  private:
    Color computeDiffuse(float lightDotN, const Material& material) const;
    Color computeSpecular(float observerDotReflected, const Material& material) const;
};

#endif