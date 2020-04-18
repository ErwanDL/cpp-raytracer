#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include "camera.hpp"
#include "light.hpp"
#include "random.hpp"
#include "shape.hpp"
#include "supersampling.hpp"

struct Color;
class Renderer {
    const Color skyColor{0.5f, 0.8f, 0.9f};

    const Intersectable &scene;
    const Light &lights;
    int width;
    int height;
    const ReflectionGenerator reflectionGenerator;
    const SupersamplingStrategy &superSampler;
    float exposure;
    float gamma;

   public:
    Renderer(const Intersectable &scene, const Light &lights, int width,
             int height, const ReflectionGenerator &reflectionGenerator,
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
};

#endif