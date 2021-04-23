#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "color.hpp"
#include <cassert>

struct Material {
    Color albedo;
    // Only allowing the "default" specularity value of most diffuse materials for now.
    float specularity = 0.04f;
    float smoothness;
    bool metal;

  public:
    Material(const Color& albedo, float smoothness = 1.0f, bool metal = false)
        : albedo(albedo), smoothness(smoothness), metal(metal) {
        assert(smoothness >= 1.0f);
    }
};

#endif