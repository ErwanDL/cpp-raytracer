#ifndef SCENE_HPP
#define SCENE_HPP

#include "color.hpp"
#include "intersectable.hpp"
#include "intersection.hpp"
#include "params.hpp"
#include "ray.hpp"
#include <memory>
#include <utility>
#include <vector>

class Scene {
    /* Holds all Intersectable objects, including the lights. */
    std::vector<std::shared_ptr<Intersectable>> intersectables;
    std::vector<std::shared_ptr<Intersectable>> lights;
    RenderParams params;

  public:
    Color skyColor;

    Scene(const std::vector<std::shared_ptr<Intersectable>>& nonLights,
          const std::vector<std::shared_ptr<Intersectable>>& lights, const RenderParams& params,
          const Color& skyColor = Color(0.7f, 0.9f, 1.0f))
        : intersectables(nonLights), lights(lights), params(params), skyColor(skyColor) {
        intersectables.insert(intersectables.end(), lights.begin(), lights.end());
    }

    Color shootRay(const Ray& ray, int remainingBounces, bool isCameraRay = false) const;
    std::optional<Intersection> findFirstIntersection(const Ray& ray) const;

  private:
    Color computeDirectDiffuseLighting(const Intersection& intersection) const;

    Color computeIndirectLighting(const Intersection& intersection, const Point3& observerLocation,
                                  int remainingBounces) const;
};

#endif