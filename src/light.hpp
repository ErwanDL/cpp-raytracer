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

class PointLight : public Light {
    Point3 origin;
    Color color;

  public:
    PointLight(const Point3& origin, const Color& color) : origin(origin), color(color) {}

    Color illuminate(const Intersection& intersection, const Scene& scene,
                     const Point3& observerLocation) const override;

  private:
    Color computeDiffuse(float lightDotN, const Material& material) const;
    Color computeSpecular(float observerDotReflected, const Material& material) const;
};

#endif