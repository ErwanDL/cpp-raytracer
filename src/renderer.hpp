#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include "camera.hpp"
#include "light.hpp"
#include "shape.hpp"

struct Color;
class Renderer {
    const Color skyColor{0.5f, 0.8f, 0.9f};

    const Intersectable& scene;
    const Light& lights;
    int width;
    int height;
    float exposure;
    float gamma;

  public:
    Renderer(const Intersectable& scene, const Light& lights, int width, int height,
             float exposure = 1.0f, float gamma = 1.0f);

    std::vector<int> rayTrace(const Camera& camera, int nBounces, int maxReflectionSamples) const;

    void saveRender(const std::vector<int>& pixelValues, const std::string& filename) const;

    Color shootRayRecursively(const Ray& ray, int nBouncesLeft, int maxReflectionSamples) const;

  private:
    Vector2 screenCoordinateFromXY(int x, int y) const;
    void setPixel(std::vector<int>& pixelValues, int row, int col, const Color& color) const;
    static void displayProgress(float progressRatio);
    static int convertTo8BitValue(float f);
};

#endif