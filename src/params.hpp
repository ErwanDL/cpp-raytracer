#ifndef PARAMS_HPP
#define PARAMS_HPP

struct RenderParams {
    int width;
    int height;
    int maxBounces;
    int nSamples;
    bool nextEventEstimation;
    // You probably only want to use that if nextEventEstimation is activated
    bool firefliesClamping;
    float gamma;

    RenderParams(int width, int height, int maxBounces, int nSamples, bool nextEventEstimation,
                 bool firefliesClamping, float gamma = 2.2f)
        : width(width), height(height), maxBounces(maxBounces), nSamples(nSamples),
          nextEventEstimation(nextEventEstimation), firefliesClamping(firefliesClamping),
          gamma(gamma) {}
};

#endif