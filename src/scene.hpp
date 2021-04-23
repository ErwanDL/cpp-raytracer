#ifndef SCENE_HPP
#define SCENE_HPP

#include "color.hpp"
#include "intersection.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include <memory>
#include <utility>
#include <vector>

class Scene {
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<Light>> lights;

  public:
    Color skyColor;

    Scene(const std::vector<std::shared_ptr<Shape>>& shapes,
          const std::vector<std::shared_ptr<Light>>& lights,
          const Color& skyColor = Color(0.7f, 0.9f, 1.0f))
        : shapes(shapes), lights(lights), skyColor(skyColor) {}

    Color shootRay(const Ray& ray, int nBounces) const;
    std::optional<Intersection> findFirstIntersection(const Ray& ray) const;

  private:
    Color computeDirectLighting(const Intersection& intersection,
                                const Point3& observerLocation) const;

    Color computeIndirectLighting(const Intersection& intersection, const Point3& observerLocation,
                                  int nBounces) const;
};

#endif