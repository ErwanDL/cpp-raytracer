#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>

struct Color final {
    float r;
    float g;
    float b;

   public:
    explicit Color(float f);
    Color(float red, float green, float blue);

    Color gammaCorrected(float exposure, float gamma) const;

    Color &operator+=(const Color &other);
    bool operator==(const Color &other) const;
};

Color operator*(const Color &c1, const Color &c2);
Color operator*(float f, const Color &c);
Color operator*(const Color &c, float f);
Color operator+(const Color &c1, const Color &c2);
std::ostream &operator<<(std::ostream &out, const Color &color);

struct Material final {
    Color color;
    float diffuse;
    float specular;
    float shininess;

    explicit Material(const Color &color = Color(0.8f, 0.2f, 0.2f),
                      float specular = 1.0f, float shininess = 60.0f,
                      float diffuse = 1.0f);
};

#endif