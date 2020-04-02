#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <vector>

#include "material.hpp"
#include "vectors.hpp"

struct Intersection;
class Scene;
class Camera;

class Light {
   public:
    virtual Color illuminate(const Intersection& intersection,
                             const Scene& scene,
                             const Point3& observerLocation) const = 0;
};

class LightRack : public Light {
    std::vector<Light*> lights{};
    Color ambient;

   public:
    explicit LightRack(const Color& ambient = Color(0.5f));

    void addLight(Light* pLight);
    Color illuminate(const Intersection& intersection, const Scene& scene,
                     const Point3& observerLocation) const override;
};

class PointLight : public Light {
    Point3 origin;
    Color color;

   public:
    PointLight(const Point3& origin, const Color& color);

    Color illuminate(const Intersection& intersection, const Scene& scene,
                     const Point3& observerLocation) const override;

   private:
    Color computeDiffuse(float lightDotN, const Material& material) const;
    Color computeSpecular(float observerDotReflected,
                          const Material& material) const;
};

#endif