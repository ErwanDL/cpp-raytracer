#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <vector>

#include "material.hpp"
#include "vectors.hpp"

struct Intersection;
class Intersectable;
class Camera;

class Light {
   public:
    virtual Color illuminate(const Intersection& intersection,
                             const Intersectable& scene,
                             const Point3& observerLocation) const = 0;
};

class LightRack : public Light {
    std::vector<const Light*> lights;

   public:
    void addLight(const Light& light);
    Color illuminate(const Intersection& intersection,
                     const Intersectable& scene,
                     const Point3& observerLocation) const override;
};

class AmbientLight : public Light {
    Color color;

   public:
    explicit AmbientLight(const Color& color);

    Color illuminate(const Intersection& intersection, const Intersectable&,
                     const Point3&) const override;
};
class PointLight : public Light {
    Point3 origin;
    Color color;

   public:
    PointLight(const Point3& origin, const Color& color);

    Color illuminate(const Intersection& intersection,
                     const Intersectable& scene,
                     const Point3& observerLocation) const override;

   private:
    Color computeDiffuse(float lightDotN, const Material& material) const;
    Color computeSpecular(float observerDotReflected,
                          const Material& material) const;
};

#endif