#include "material.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

// STRUCT COLOR

Color::Color() {}
Color::Color(float f) {
    assert(
        f <= 1.0f &&
        "Floats given to the Color constructor must be between 0.0f and 1.0f");
    r = f;
    g = f;
    b = f;
}
Color::Color(float red, float green, float blue) {
    assert(red <= 1.0f && blue <= 1.0f && green <= 1.0f &&
           "Floats given to the Color constructor must be "
           "between 0.0f and 1.0f");
    r = red;
    g = green;
    b = blue;
}

Color &Color::clamp(float min, float max) {
    r = std::clamp(r, min, max);
    g = std::clamp(g, min, max);
    b = std::clamp(b, min, max);
    return *this;
}

Color &Color::applyGamma(float exposure, float gamma) {
    r = std::pow(r * exposure, gamma);
    g = std::pow(g * exposure, gamma);
    b = std::pow(b * exposure, gamma);
    return *this;
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

// STRUCT MATERIAL

Material::Material() {}

Material::Material(const Color &color, float specular, float shininess,
                   float diffuse, float ambient)
    : color(color),
      ambient(ambient),
      diffuse(diffuse),
      specular(specular),
      shininess(shininess) {}