#ifndef TRACE_HPP
#define TRACE_HPP

#include "camera.hpp"
#include "scene.hpp"
#include <utility>
#include <vector>

struct RenderParams {
    int width;
    int height;
    int maxBounces;
    int nSamples;
    float gamma;

    RenderParams(int width, int height, int maxBounces, int nSamples, float gamma = 2.2f)
        : width(width), height(height), maxBounces(maxBounces), nSamples(nSamples), gamma(gamma) {}
};

std::vector<std::vector<Color>> rayTrace(const PerspectiveCamera& camera, const Scene& scene,
                                         const RenderParams& params);

#endif