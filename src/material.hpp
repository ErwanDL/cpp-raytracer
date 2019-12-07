#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>

struct Color final {
    float r{0.0f};
    float g{0.0f};
    float b{0.0f};

    Color();
    Color(float l);
    Color(float red, float green, float blue);

    Color &clamp(float min = 0.0f, float max = 1.0f);
    Color &applyGamma(float exposure, float gamma);

    Color &operator+=(const Color &other);
};

Color operator*(const Color &c1, const Color &c2);
Color operator*(float f, const Color &c);
Color operator*(const Color &c, float f);
Color operator+(const Color &c1, const Color &c2);
std::ostream &operator<<(std::ostream &out, const Color &color);

struct Material final {
    Color color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;

    Material(const Color &color = Color(0.8f, 0.2f, 0.2f),
             float specular = 1.0f, float shininess = 60.0f,
             float diffuse = 1.0f, float ambient = 1.0f);
};

#endif