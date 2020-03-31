#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include "camera.hpp"

class Color;
class Scene;
class LightRack;
class Renderer {
    int width;
    int height;
    float exposure;
    float gamma;

   public:
    Renderer(int width, int height, float exposure = 1.0f, float gamma = 2.2f);

    void setPixel(std::vector<int> &pixelValues, int row, int col,
                  const Color &color);

    std::vector<int> rayTrace(const Camera &camera, Scene &scene,
                              const LightRack &lightRack);

    void saveRenderer(const std::vector<int> &pixelValues,
                      const std::string &filename) const;
};

#endif