#include "material.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "utils.hpp"

Color::Color(float f) : r(f), g(f), b(f) {}
Color::Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

Color Color::gammaCorrected(float exposure, float gamma) const {
    float red = Math::unitClamp(std::pow(r * exposure, gamma));
    float green = Math::unitClamp(std::pow(g * exposure, gamma));
    float blue = Math::unitClamp(std::pow(b * exposure, gamma));
    return Color(red, green, blue);
}

Color operator*(const Color &c1, const Color &c2) {
    return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}
Color operator*(float f, const Color &c) {
    return Color(c.r * f, c.g * f, c.b * f);
}
Color operator*(const Color &c, float f) {
    return Color(c.r * f, c.g * f, c.b * f);
}
Color operator+(const Color &c1, const Color &c2) {
    return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}
std::ostream &operator<<(std::ostream &out, const Color &color) {
    out << "Color(R: " << color.r << ", G: " << color.g << ", B: " << color.b
        << ')';
    return out;
}

Color &Color::operator+=(const Color &other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

bool Color::operator==(const Color &other) const {
    return Math::floatingPointEquality(r, other.r) &&
           Math::floatingPointEquality(g, other.g) &&
           Math::floatingPointEquality(b, other.b);
}

// MATERIALS
Material::Material(const Color &diffuseColor, const Color &specularColor,
                   float smoothness)
    : diffuseColor(diffuseColor),
      specularColor(specularColor),
      smoothness(smoothness) {}

bool Material::isReflective() const {
    return specularColor.r > 0.0f || specularColor.g > 0.0f ||
           specularColor.b > 0.0f;
}

Lambertian::Lambertian(const Color &diffuseColor, float specularity,
                       float smoothness)
    : Material(diffuseColor, Color(specularity), smoothness) {}

Metal::Metal(const Color &specularColor, float smoothness)
    : Material(Color(0.0f), specularColor, smoothness) {}
