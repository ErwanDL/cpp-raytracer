#ifndef PARAMS_HPP
#define PARAMS_HPP

struct RenderParams {
    int width;
    int height;
    int maxBounces;
    int nSamples;
    bool nextEventEstimation;
    float gamma;

    RenderParams(int width, int height, int maxBounces, int nSamples,
                 bool nextEventEstimation = true, float gamma = 2.2f)
        : width(width), height(height), maxBounces(maxBounces), nSamples(nSamples),
          nextEventEstimation(nextEventEstimation), gamma(gamma) {}
};

#endif