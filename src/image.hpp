#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>
#include "camera.hpp"

class Color;
class Scene;
class Light;
class Image {
   protected:
    int width;
    int height;
    std::vector<int> data;
    float exposure;
    float gamma;

   public:
    Image(int width, int height, float exposure = 1.0f, float gamma = 2.2f);

    void setPixel(int row, int col, const Color &color);

    void rayTrace(const Camera &camera, Scene &scene, const Light &light);

    void saveImage(const std::string &filename) const;
};

#endif