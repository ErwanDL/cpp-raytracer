#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>

struct Color {
    float r;
    float g;
    float b;

  public:
    Color(float f);
    Color(float red, float green, float blue);

    Color gammaCorrected(float exposure, float gamma) const;

    Color& operator+=(const Color& other);
    Color& operator/=(float f);
    bool operator==(const Color& other) const;

    static Color WHITE;
    static Color BLACK;
};

Color operator*(const Color& c1, const Color& c2);
Color operator*(float f, const Color& c);
Color operator*(const Color& c, float f);
Color operator/(const Color& c, float f);
Color operator+(const Color& c1, const Color& c2);
std::ostream& operator<<(std::ostream& out, const Color& color);

struct Material {
    Color albedo;
    // Only allowing the "default" specularity value of most diffuse materials for now.
    float specularity = 0.04f;
    float smoothness;
    bool metal;

  public:
    Material(const Color& diffuseColor, float smoothness = 1.0f, bool metal = false);
};

#endif