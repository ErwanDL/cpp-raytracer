#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <utility>
#include <vector>

#include "camera.hpp"
#include "light.hpp"
#include "random.hpp"
#include "shape.hpp"

struct Color;
class SupersamplingStrategy;
class Renderer {
    const Color skyColor{0.5f, 0.8f, 0.9f};

    const Intersectable &scene;
    const Light &lights;
    int width;
    int height;
    int maxGlossySamples;
    const SupersamplingStrategy &superSampler;
    float exposure;
    float gamma;
    UniformIntervalRNG generator{-1.0f, 1.0f};

   public:
    Renderer(const Intersectable &scene, const Light &lights, int width,
             int height, int maxGlossySamples,
             const SupersamplingStrategy &superSampler, float exposure = 1.0f,
             float gamma = 2.2f);

    std::vector<int> rayTrace(const Camera &camera, int nBounces) const;

    void saveRender(const std::vector<int> &pixelValues,
                    const std::string &filename) const;

    Color shootRayRecursively(const Ray &ray, int nBouncesLeft) const;

   private:
    Vector2 screenCoordinateFromXY(float x, float y) const;
    void setPixel(std::vector<int> &pixelValues, int row, int col,
                  const Color &color) const;
    static void displayProgress(float progressRatio);
    static int convertTo8BitValue(float f);
    std::vector<Vector3> getRandomReflections(int nReflections,
                                              const Vector3 mainDirection,
                                              float smoothness) const;
    static float toAngle(float f, float exponent);
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
    UniformIntervalRNG generator{-0.5f, 0.5f};

   public:
    explicit StochasticSupersampler(int samplesPerPixel);
    std::vector<std::pair<float, float>> getSupersamplingOffsets()
        const override;
};
#endif