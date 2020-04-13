#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <random>
#include <string>
#include <utility>
#include <vector>

#include "camera.hpp"
#include "light.hpp"
#include "shape.hpp"

struct Color;
class SupersamplingStrategy;
class Renderer {
    const Color skyColor{0.5f, 0.8f, 0.9f};

    const Intersectable &scene;
    const Light &lights;
    int width;
    int height;
    const SupersamplingStrategy &superSampler;
    float exposure;
    float gamma;

   public:
    Renderer(const Intersectable &scene, const Light &lights, int width,
             int height, const SupersamplingStrategy &superSampler,
             float exposure = 1.0f, float gamma = 2.2f);

    std::vector<int> rayTrace(const Camera &camera, int nReflexions) const;

    void saveRenderer(const std::vector<int> &pixelValues,
                      const std::string &filename) const;

    Color shootRayRecursively(const Ray &ray, int nReflexionsLeft) const;

   private:
    Vector2 screenCoordinateFromXY(float x, float y) const;
    void setPixel(std::vector<int> &pixelValues, int row, int col,
                  const Color &color) const;
    static int convertTo8BitValue(float f);
};

class SupersamplingStrategy {
   public:
    virtual std::vector<std::pair<float, float>> getSupersamplingOffsets()
        const = 0;
};

class DeterministicSupersampler : public SupersamplingStrategy {
    int rate;

   public:
    explicit DeterministicSupersampler(int rate);
    std::vector<std::pair<float, float>> getSupersamplingOffsets()
        const override;
};

class StochasticSupersampler : public SupersamplingStrategy {
    int samplesPerPixel;
    mutable std::mt19937 generator;
    mutable std::uniform_real_distribution<float> distribution{-0.5, 0.5f};

   public:
    explicit StochasticSupersampler(int samplesPerPixel);
    std::vector<std::pair<float, float>> getSupersamplingOffsets()
        const override;
};
#endif