#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>
#include "camera.hpp"

class Color final {
   public:
    float r{0.0f};
    float g{0.0f};
    float b{0.0f};

    Color();
    Color(float l);
    Color(float red, float green, float blue);

    Color &clamp(float min = 0.0f, float max = 1.0f);
    Color &applyGamma(float exposure, float gamma);
};

class Shape;
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

    void rayTrace(const Camera &camera, Shape &scene);

    void saveImage(const std::string &filename) const;
};

#endif