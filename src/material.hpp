#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "color.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include <cassert>
#include <cmath>
#include <utility>
enum MaterialType { Diffuse, Metal, Emissive, Refractive };

struct Material {
    Color color;
    // Specularity adds a lot of fireflies to the scene : removing it for diffuse materials for now.
    float specularity = 0.0f;
    float smoothness;
    MaterialType type;
    float emission;
    float IOR;

  private:
    Material(const Color& color, float smoothness, MaterialType type, float emission, float IOR)
        : color(color), smoothness(smoothness), type(type), emission(emission), IOR(IOR) {
        assert(smoothness >= 1.0f);
        assert(emission >= 0.0f);
    }

  public:
    static Material Metal(const Color& color, float smoothness) {
        return Material(color, smoothness, MaterialType::Metal, 0.0f, 1.0f);
    }

    static Material Diffuse(const Color& color, float smoothness = 1.0f) {
        return Material(color, smoothness, MaterialType::Diffuse, 0.0f, 1.0f);
    }

    static Material Emissive(const Color& color, float emission) {
        return Material(color, 1.0f, MaterialType::Emissive, emission, 1.0F);
    }

    static Material Refractive(const Color& color, float IOR = 1.5f) {
        return Material(color, 1.0f, MaterialType::Refractive, 0.0f, IOR);
    }
};

std::pair<Ray, Color> reflectOrRefract(const Intersection& intersection, const Point3& rayOrigin);

#endif