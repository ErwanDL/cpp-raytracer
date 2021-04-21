#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>

struct Color final {
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
};

Color operator*(const Color& c1, const Color& c2);
Color operator*(float f, const Color& c);
Color operator*(const Color& c, float f);
Color operator/(const Color& c, float f);
Color operator+(const Color& c1, const Color& c2);
std::ostream& operator<<(std::ostream& out, const Color& color);

struct Material {
    Color diffuseColor;
    Color specularColor;
    float specularity;
    float roughness;

  protected:
    Material(const Color& diffuseColor, const Color& specularColor, float specularity,
             float roughness);
};

struct Lambertian : public Material {
    Lambertian(const Color& diffuseColor, float specularity = 0.05f, float roughness = 0.5f);
};

struct Metal : public Material {
    Metal(const Color& specularColor, float roughness = 0.0f);
};

#endif