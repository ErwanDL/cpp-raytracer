#include "material.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "../src/utils.hpp"

Color::Color(float f) {
    const float clampedF = Math::unitClamp(f);
    r = clampedF;
    g = clampedF;
    b = clampedF;
}
Color::Color(float red, float green, float blue) {
    r = Math::unitClamp(red);
    g = Math::unitClamp(green);
    b = Math::unitClamp(blue);
}

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
    return Color(Math::unitClamp(c1.r + c2.r), Math::unitClamp(c1.g + c2.g),
                 Math::unitClamp(c1.b + c2.b));
}
std::ostream &operator<<(std::ostream &out, const Color &color) {
    out << "Color(R: " << color.r << ", G: " << color.g << ", B: " << color.b
        << ')';
    return out;
}

Color &Color::operator+=(const Color &other) {
    r = Math::unitClamp(r + other.r);
    g += Math::unitClamp(other.g);
    b += Math::unitClamp(other.b);
    return *this;
}

bool Color::operator==(const Color &other) const {
    return Math::floatingPointEquality(r, other.r) &&
           Math::floatingPointEquality(g, other.g) &&
           Math::floatingPointEquality(b, other.b);
}

// STRUCT MATERIAL

Material::Material(const Color &color, float specular, float shininess,
                   float diffuse)
    : color(color),
      diffuse(diffuse),
      specular(specular),
      shininess(shininess) {}