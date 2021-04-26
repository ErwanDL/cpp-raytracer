#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "color.hpp"
#include <cassert>

struct Material {
    Color color;
    // Only allowing the "default" specularity value of most diffuse materials for now.
    float specularity = 0.0f;
    float smoothness;
    bool metal;
    float emission = 0.0f;

  private:
    Material(const Color& color, float smoothness, bool metal, float emission)
        : color(color), smoothness(smoothness), metal(metal), emission(emission) {
        assert(smoothness >= 1.0f);
        assert(emission >= 0.0f);
    }

  public:
    static Material Metal(const Color& color, float smoothness) {
        return Material(color, smoothness, true, 0.0f);
    }

    static Material Diffuse(const Color& color, float smoothness = 1.0f) {
        return Material(color, smoothness, false, 0.0f);
    }

    static Material Emissive(const Color& color, float emission) {
        return Material(color, 1.0f, false, emission);
    }
};

#endif