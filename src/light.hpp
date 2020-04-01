#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <vector>

#include "material.hpp"
#include "vectors.hpp"

class Intersection;
class Scene;
class Camera;

class ILight {
   public:
    virtual ~ILight() = default;

    virtual Color illuminate(const Intersection& intersection,
                             const Scene& scene, const Camera& cam) const = 0;
};

class LightRack : public ILight {
    std::vector<ILight*> lights{};
    Color ambient;

   public:
    explicit LightRack(const Color& ambient = 0.5f);

    void addLight(ILight* pLight);
    Color illuminate(const Intersection& intersection, const Scene& scene,
                     const Camera& cam) const override;
};

class PointLight : public ILight {
    Point3 origin{0.0f, 5.0f, -1.0f};
    Color color{0.5f};

   public:
    PointLight(const Point3& origin, const Color& color);

    Color illuminate(const Intersection& intersection, const Scene& scene,
                     const Camera& cam) const override;
};

#endif