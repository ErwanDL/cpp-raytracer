#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include "camera.hpp"
#include "light.hpp"
#include "shape.hpp"

struct Color;
class Renderer {
    const Intersectable &scene;
    const Light &lights;
    int width;
    int height;
    float exposure;
    float gamma;

   public:
    Renderer(const Intersectable &scene, const Light &lights, int width,
             int height, float exposure = 1.0f, float gamma = 2.2f);

    std::vector<int> rayTrace(const Camera &camera);

    void saveRenderer(const std::vector<int> &pixelValues,
                      const std::string &filename) const;

   private:
    Vector2 screenCoordinateFromXY(int x, int y) const;
    void setPixel(std::vector<int> &pixelValues, int row, int col,
                  const Color &color);
    static int convertTo8BitValue(float f);

    Color shootRayRecursively(const Ray &ray, int nReflexions) const;
};

#endif