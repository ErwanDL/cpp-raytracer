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

    Color &operator+=(const Color &other);
    bool operator==(const Color &other) const;
};

Color operator*(const Color &c1, const Color &c2);
Color operator*(float f, const Color &c);
Color operator*(const Color &c, float f);
Color operator+(const Color &c1, const Color &c2);
std::ostream &operator<<(std::ostream &out, const Color &color);

struct Material {
    Color diffuseColor;
    Color specularColor;
    float smoothness;

    bool isReflective() const;

   protected:
    Material(const Color &diffuseColor, const Color &specularColor,
             float smoothness);
};

struct Lambertian : public Material {
    Lambertian(const Color &diffuseColor, float specularity = 0.0f,
               float smoothness = 50.0f);
};

struct Metal : public Material {
    Metal(const Color &specularColor, float smoothness = 100.0f);
};

#endif