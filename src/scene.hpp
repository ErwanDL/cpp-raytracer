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
          const Color& skyColor = Color(0.5f, 0.8f, 0.9f))
        : shapes(shapes), lights(lights), skyColor(skyColor) {}

    std::optional<Intersection> findFirstIntersection(const Ray& ray) const {
        std::optional<Intersection> closestIntersection;

        for (const auto& shape : shapes) {
            auto intersection = shape->intersect(ray);
            if (intersection &&
                (!closestIntersection ||
                 intersection->distanceToRayOrigin < closestIntersection->distanceToRayOrigin)) {
                closestIntersection = intersection;
            }
        }

        return closestIntersection;
    }

    Color computeTotalLighting(const Intersection& intersection,
                               const Point3& observerLocation) const {
        Color intersectionColor{0.0f};
        for (const auto& light : lights) {
            intersectionColor += light->illuminate(intersection, *this, observerLocation);
        }
        return intersectionColor;
    }
};

#endif